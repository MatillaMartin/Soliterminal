#include "Card.h"
#include "CardStack.h"
#include "Game.h"
#include "GameControl.h"
#include "GameLayout.h"
#include "Render.h"
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

int main()
{
	try
	{
		Console* console = nullptr;
#ifdef WIN32
		ConsoleWindows consoleWin;
		console = &consoleWin;
#else
		ConsoleLinux consoleLinux;
		console = &consoleLinux;
#endif
		assert(console != nullptr); // Console not initialized
		if (!console)
			return -1;

		Game game = createGame();
		GameLayout gameLayout;
		GameControl control(game, gameLayout);
		Render render(game, control, gameLayout, *console);

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