#include "App.h"
#include "AppControl.h"
#include "AppRender.h"
#include "Card.h"
#include "CardStack.h"
#include "Game.h"
#include "GameControl.h"
#include "GameFileIO.h"
#include "GameRender.h"
#include "Layout.h"
#include "Menu.h"
#include "MenuControl.h"
#include "MenuRender.h"
#include "MenuSelection.h"
#include "UserInput.h"

#ifdef WIN32
#	include "ConsoleWindows.h"
#else
#	include "ConsoleLinux.h"
#endif

#include <algorithm>
#include <array>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <thread>

using namespace panda;

Layout createGameLayout()
{
	// map game to the layout, where top row contains open, closed, end stacks, and bottom row contains central stacks
	// bottom row is one index down
	// layout is as follows:
	// 0:closed	| 1:open	| -			| 2:end0	| 3:end1	| 4:end2	| 5:end3	|
	// 6:cen0	| 7:cen1	| 8:cen2	| 9:cen3	| 10:cen4	| 11:cen5	| 12:cen6	|
	Graph graph;
	graph.addNode(0, {0, 0});
	graph.addNode(1, {1, 0});
	graph.addNode(2, {3, 0});
	graph.addNode(3, {4, 0});
	graph.addNode(4, {5, 0});
	graph.addNode(5, {6, 0});
	graph.addNode(6, {0, 1});
	graph.addNode(7, {1, 1});
	graph.addNode(8, {2, 1});
	graph.addNode(9, {3, 1});
	graph.addNode(10, {4, 1});
	graph.addNode(11, {5, 1});
	graph.addNode(12, {6, 1});

	graph.addHorChain({0, 1, 2, 3, 4, 5});
	graph.addHorChain({6, 7, 8, 9, 10, 11, 12});

	graph.addVerEdge(0, 6);
	graph.addVerEdge(1, 7);
	graph.addVerEdge(2, 9);
	graph.addVerEdge(3, 10);
	graph.addVerEdge(4, 11);
	graph.addVerEdge(5, 12);

	// add an edge that only goes up
	graph.addUpEdge(8, 1);
	return Layout(std::move(graph));
}

Game loadOrCreateGame()
{
	// Try to load game if one exists already
	if (GameFileIO::hasSavedGame())
	{
		auto game = GameFileIO::loadGame();
		if (game)
			return *game;
	}

	return Game::createRandomGame();
}

std::unique_ptr<Console> consoleProxy()
{
#ifdef WIN32
	try
	{
		return std::make_unique<ConsoleWindows>();
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "Failed to create windows console proxy: " << e.what();
		return {};
	}
#else
	return std::make_unique<ConsoleLinux>();
#endif
	return {};
}

int main()
{
	try
	{
		std::unique_ptr<Console> console = consoleProxy();
		assert(console != nullptr);    // Console not initialized
		if (!console)
			return -1;

		Game game = loadOrCreateGame();

		App app;

		Layout gameLayout = createGameLayout();
		GameControl gameControl(game, gameLayout);

		Menu menu{"Soliterminal",
				  "",
				  {{"Resume", [&app]() { app.setState(App::State::Game); }},
				   {"New Game",
					[&app, &game, &gameControl]() {
						game.reset(Game::createRandomGame());
						gameControl.reset();
						app.setState(App::State::Game);
					}},
				   {"Save and Exit",
					[&app, &game]() {
						GameFileIO::saveGame(game);
						app.setState(App::State::Exit);
					}},
				   {"Exit without saving", [&app]() { app.setState(App::State::Exit); }}}};

		MenuSelection menuSelection;
		MenuControl menuControl(menu, menuSelection);
		GameRender gameRender(game, gameControl, gameLayout, *console);
		MenuRender menuRender(menu, menuSelection, *console);

		AppControl appControl(app, AppControl::Controls{gameControl, menuControl});
		AppRender appRender(app, AppRender::Renders{gameRender, menuRender});

		// Basic application cycle
		while (true)
		{
			if (app.state() == App::State::Exit)
				break;

			Action action = UserInput::waitForInput();
			appControl.action(action);
			appRender.update();
		}

		console->clear();
	}
	catch (std::runtime_error& e)
	{
		std::cout << "Unexpected error: " << e.what();
		return -1;
	}

	return 0;
}