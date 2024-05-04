#include "Card.h"
#include "CardStack.h"
#include "Game.h"
#include "GameControl.h"
#include "GameFileIO.h"
#include "GameRender.h"
#include "Layout.h"
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
#include <random>
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

Layout createMenuLayout()
{
	Graph graph;
	graph.addNode(0, {0, 0});
	graph.addNode(1, {0, 1});
	graph.addNode(2, {0, 2});
	graph.addVerChain({0, 1, 2, 0});
	return Layout(std::move(graph));
}

std::vector<Card> createDeck()
{
	std::vector<Card> deck(52);
	for (int suitIndex = 0; suitIndex < 4; ++suitIndex)
	{
		for (int numberIndex = 0; numberIndex < 13; ++numberIndex)
		{
			int cardIndex = numberIndex + suitIndex * 13;
			assert(cardIndex >= 0 && cardIndex < deck.size());
			Card& card = deck[cardIndex];
			card.number = numberIndex + 1;    // card numbers start on 1
			card.suit = static_cast<Card::Suit>(suitIndex);
			card.state = Card::State::Closed;
		}
	}
	return deck;
}

Game createGame()
{
	// start with a deck
	auto deck = createDeck();

	// random shuffle
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(deck.begin(), deck.end(), g);

	// separate cards into stacks

	// Empty end stack
	std::array<CardStack, 4> endStack;

	// Central stack with some cards
	std::array<CardStack, 7> centralStack;

	// take incrementally more cards, and open the first card of each
	for (int i = 0; i < centralStack.size(); ++i)
	{
		int cardsToTake = i + 2;
		std::vector<Card> cards;
		auto cardIt = deck.end() - cardsToTake;
		// move cards into separate vector
		cards.insert(cards.end(), std::make_move_iterator(cardIt), std::make_move_iterator(deck.end()));
		// remove moved cards from deck
		deck.erase(cardIt, deck.end());

		CardStack stack(std::move(cards));
		stack.flipTop();
		centralStack[i] = std::move(stack);
	}

	assert(deck.size() == 17);

	CardStack closedStack(std::move(deck));    // closed stack are the left over cards

	CardStack openStack{std::vector<Card>{}};
	Game::Stacks state(std::move(endStack), std::move(centralStack), std::move(closedStack), std::move(openStack));

	// create a fixed state for now
	return Game(std::move(state));
}

Game createNearEndingGame()
{
	// start with a deck
	auto deck = createDeck();

	// Full end stack
	std::array<CardStack, 4> endStack;
	for (int suitIndex = 0; suitIndex < 4; ++suitIndex)
	{
		auto cardBegin = deck.begin();
		std::advance(cardBegin, suitIndex * 13);
		auto cardEnd = cardBegin;
		std::advance(cardEnd, 13);
		std::vector<Card> cards(cardBegin, cardEnd);
		endStack[suitIndex] = CardStack(std::move(cards));
		endStack[suitIndex].flipAll();
	}

	// Rest empty
	std::array<CardStack, 7> centralStack;
	CardStack closedStack;    // closed stack are the left over cards
	CardStack openStack;
	Game::Stacks state(std::move(endStack), std::move(centralStack), std::move(closedStack), std::move(openStack));

	// create a fixed state for now
	return Game(std::move(state));
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

		Layout gameLayout = createGameLayout();
		Game game = createGame();
		GameControl control(game, gameLayout);
		GameRender render(game, control, gameLayout, *console);

		// Basic rendering cycle
		while (true)
		{
			GameAction action = UserInput::waitForInput();
			if (action == GameAction::None)
				continue;
			if (action == GameAction::Reset)
				game.reset(createGame());
			if (action == GameAction::Exit)
			{
				console->clear();
				return 0;
			}

			control.action(action);

			render.update();

			game.checkWin();
			if (game.state() != Game::State::Playing)
			{
				break;
			}
		}
	}
	catch (std::runtime_error& e)
	{
		std::cout << "Unexpected error: " << e.what();
		return -1;
	}

	return 0;
}