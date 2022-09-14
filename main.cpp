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
			Card(0, Card::Suit::Club, Card::State::Open),
		},
		std::vector<Card>{
			Card(0, Card::Suit::Club, Card::State::Open),
		},
		std::vector<Card>{
			Card(0, Card::Suit::Club, Card::State::Open),
		},
		std::vector<Card>{
			Card(0, Card::Suit::Club, Card::State::Open),
		},
		std::vector<Card>{
			Card(0, Card::Suit::Club, Card::State::Open),
		},
		std::vector<Card>{
			Card(0, Card::Suit::Club, Card::State::Open),
		},
	};

	CardStack closedStack{std::vector<Card>{
		Card(0, Card::Suit::Club, Card::State::Closed),
		Card(1, Card::Suit::Club, Card::State::Closed),
		Card(2, Card::Suit::Club, Card::State::Closed),
	}};

	CardStack openStack{std::vector<Card>{}};

	Game::Stacks state(endStack, centralStack, closedStack, openStack);

	// create a fixed state for now
	return Game(state);
}

int main()
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

	return 0;
}