#include "Card.h"
#include "CardStack.h"
#include "Game.h"
#include "GameControl.h"
#include "GameLayout.h"
#include "Render.h"
#include "UserInput.h"

#include <array>
#include <chrono>
#include <iostream>
#include <thread>

using namespace panda;

Game createGame()
{
	// start with a deck

	// shuffle

	//

	// Empty end stack
	std::array<CardStack, 4> endStack{
		std::vector<Card>{},
		std::vector<Card>{

		},
		std::vector<Card>{

		},
		std::vector<Card>{

		},
	};

	// Central stack with some cards
	std::array<CardStack, 7> centralStack{
		std::vector<Card>{
			Card(13, Card::Suit::Heart, Card::State::Closed),

		},
		std::vector<Card>{
			Card(1, Card::Suit::Heart, Card::State::Closed),
			Card(2, Card::Suit::Heart, Card::State::Closed),
			Card(3, Card::Suit::Heart, Card::State::Closed),
			Card(13, Card::Suit::Spade, Card::State::Open),
			Card(13, Card::Suit::Diamond, Card::State::Open),
			Card(13, Card::Suit::Spade, Card::State::Open),

		},
		std::vector<Card>{
			Card(13, Card::Suit::Heart, Card::State::Closed),
			Card(13, Card::Suit::Diamond, Card::State::Open),
			Card(13, Card::Suit::Heart, Card::State::Open),
			Card(13, Card::Suit::Diamond, Card::State::Open),
			Card(13, Card::Suit::Heart, Card::State::Open),
			Card(13, Card::Suit::Diamond, Card::State::Open),

		},
		std::vector<Card>{
			Card(13, Card::Suit::Club, Card::State::Open),

		},
		std::vector<Card>{},
		std::vector<Card>{},
		std::vector<Card>{},
	};

	CardStack closedStack{std::vector<Card>{
		Card(11, Card::Suit::Club, Card::State::Closed),
		Card(12, Card::Suit::Diamond, Card::State::Closed),
		Card(13, Card::Suit::Club, Card::State::Closed),
		Card(9, Card::Suit::Diamond, Card::State::Closed),
		Card(10, Card::Suit::Club, Card::State::Closed),
		Card(11, Card::Suit::Diamond, Card::State::Closed),
	}};

	CardStack openStack{std::vector<Card>{}};

	Game::Stacks state(std::move(endStack), std::move(centralStack), std::move(closedStack), std::move(openStack));

	// create a fixed state for now
	return Game(std::move(state));
}

int main()
{
	try
	{
		Game game = createGame();
		GameLayout gameLayout;
		GameControl control(game, gameLayout);
		Render render(game, control, gameLayout);

		// Basic rendering cycle
		while (true)
		{
			GameAction action = UserInput::waitForInput();
			if (action == GameAction::None)
				continue;
			if (action == GameAction::Reset)
				game.reset(createGame());

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