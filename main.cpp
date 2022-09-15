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
	// Empty end stack
	std::array<CardStack, 4> endStack{
		std::vector<Card>{},
		std::vector<Card>{},
		std::vector<Card>{},
		std::vector<Card>{},
	};

	// Central stack with some cards
	std::array<CardStack, 6> centralStack{
		std::vector<Card>{
			Card(13, Card::Suit::Club, Card::State::Open),
			Card(12, Card::Suit::Diamond, Card::State::Open),
			Card(11, Card::Suit::Spade, Card::State::Open),
			Card(10, Card::Suit::Heart, Card::State::Open),
		},
		std::vector<Card>{
			Card(13, Card::Suit::Diamond, Card::State::Open),
			Card(12, Card::Suit::Club, Card::State::Open),
			Card(11, Card::Suit::Heart, Card::State::Open),
			Card(10, Card::Suit::Spade, Card::State::Open),
		},
		std::vector<Card>{
			Card(13, Card::Suit::Diamond, Card::State::Open),
			Card(12, Card::Suit::Club, Card::State::Open),
			Card(11, Card::Suit::Heart, Card::State::Open),
			Card(10, Card::Suit::Spade, Card::State::Open),
			Card(9, Card::Suit::Heart, Card::State::Open),
			Card(8, Card::Suit::Club, Card::State::Open),
			Card(7, Card::Suit::Heart, Card::State::Open),
			Card(6, Card::Suit::Spade, Card::State::Open),
			Card(5, Card::Suit::Diamond, Card::State::Open),
			Card(4, Card::Suit::Spade, Card::State::Open),
			Card(3, Card::Suit::Diamond, Card::State::Open),
			Card(2, Card::Suit::Spade, Card::State::Open),
			Card(1, Card::Suit::Diamond, Card::State::Open),
		},
		std::vector<Card>{
			Card(3, Card::Suit::Club, Card::State::Open),
		},
		std::vector<Card>{
			Card(4, Card::Suit::Club, Card::State::Open),
		},
		std::vector<Card>{
			Card(5, Card::Suit::Club, Card::State::Open),
		},
	};

	CardStack closedStack{std::vector<Card>{
		Card(6, Card::Suit::Club, Card::State::Closed),
		Card(7, Card::Suit::Club, Card::State::Closed),
		Card(8, Card::Suit::Club, Card::State::Closed),
	}};

	CardStack openStack{std::vector<Card>{}};

	Game::Stacks state(endStack, centralStack, closedStack, openStack);

	// create a fixed state for now
	return Game(state);
}

int main()
{
	try
	{
		Game game = createGame();
		GameLayout gameLayout(game);
		GameControl control(game, gameLayout);
		Render render(control, gameLayout);

		// Basic rendering cycle
		while (true)
		{
			GameAction action = UserInput::waitForInput();
			if (action == GameAction::None)
				continue;

			control.action(action);

			render.update();
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