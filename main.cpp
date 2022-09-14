#include "Card.h"
#include "CardStack.h"
#include "Game.h"
#include "Render.h"

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
		Render render(game);

		// Basic rendering cycle
		while (true)
		{
			render.update();
			std::this_thread::sleep_for(std::chrono::milliseconds(16));

			if (game.state() != Game::State::Playing)
			{
				break;
			}
		}
	}
	catch (std::runtime_error& e)
	{
		std::cout << "Unexpected error: " << e.what();
	}

	return 0;
}