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
			Stacks(std::array<CardStack, 4>&& endStack, std::array<CardStack, 6>&& centralStack, CardStack&& closedStack, CardStack&& openStack)
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

		Game(Stacks&& state);

		const std::vector<CardStack>& stacks() const { return m_stacks; }

		// Returns the game state
		State state() const { return m_state; }

		// Game operations on the game state
		/// Moves a card from closed to open stack
		void openCard();

		/// Moves all the open cards from open to closed stack
		void resetClosedStack();

		/// Moves cards between specified stacks
		/// Returns false if was not able to move those cards
		bool moveCards(int originStack, int cardOriginIndex, int destStack);

		// Returns true if the index matches an end stack
		bool isEndStack(int index) const;

		// Returns true if the index matches a central stack
		bool isCentralStack(int index) const;

		// Returns true if the index matches the OpenStack
		bool isOpenStack(int index) const;

		// Returns true if the index matches the ClosedStack
		bool isClosedStack(int index) const;

		// Returns the index for the OpenStack
		int openStackIndex() const;

		// Returns the index for the ClosedStack
		int closedStackIndex() const;	

		// Returns the indices for CentralStacks
		std::vector<int> centralStacksIndices() const;

		// Returns the indices for EndStacks
		std::vector<int> endStacksIndices() const;

	private:
		std::vector<CardStack> m_stacks;
		State m_state = State::Playing;
	};
}