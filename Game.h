#pragma once
#include "CardStack.h"

#include <array>

namespace panda
{
	class Game
	{
	public:
		struct Stacks
		{
			Stacks(std::array<CardStack, 4> endStack, std::array<CardStack, 6> centralStack, CardStack closedStack, CardStack openStack)
				: endStack(endStack)
				, centralStack(centralStack)
				, closedStack(closedStack)
				, openStack(openStack)
			{
			}
			std::array<CardStack, 4> endStack;
			std::array<CardStack, 6> centralStack;
			CardStack closedStack;
			CardStack openStack;
		};

		enum class State
		{
			Playing,
			Win,
			Lose
		};

		Game(Stacks state);

		// Returns the game stacks
		const Stacks& stacks() const;

		// Returns the game state
		State state() const { return m_state; }

		// Game operations on the game state
		/// Moves a card from closed to open stack
		void openCard();

		/// Moves all the open cards from open to closed stack
		void resetOpenClosedStack();

		/// Moves cards between specified stacks
		/// Returns false if was not able to move those cards
		bool moveCards(CardStack& origin, int cardOriginIndex, CardStack& dest);

	private:
		Stacks m_stacks;
		State m_state = State::Playing;
		bool m_isDirty = true;
	};
}