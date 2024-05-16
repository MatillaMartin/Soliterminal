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
			Stacks() = default;
			Stacks(std::array<CardStack, 4>&& endStack, std::array<CardStack, 7>&& centralStack, CardStack&& closedStack, CardStack&& openStack)
				: endStack(endStack)
				, centralStack(centralStack)
				, closedStack(closedStack)
				, openStack(openStack)
			{
			}
			std::array<CardStack, 4> endStack;
			std::array<CardStack, 7> centralStack;
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

		static Game createRandomGame();

		static Game createNearEndingGame();

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
		bool moveCards(size_t originStack, size_t cardOriginIndex, size_t destStack);

		/// Returns if the card is a flipped card
		bool isFlippedCard(size_t stack, size_t cardIndex);

		/// Flips the card
		/// Returns if the card could be flipped
		bool flipCard(size_t stack, size_t cardIndex);

		// Returns true if the index matches an end stack
		bool isEndStack(size_t index) const;

		// Returns true if the index matches a central stack
		bool isCentralStack(size_t index) const;

		// Returns true if the index matches the OpenStack
		bool isOpenStack(size_t index) const;

		// Returns true if the index matches the ClosedStack
		bool isClosedStack(size_t index) const;

		// Returns the indices for CentralStacks
		std::array<size_t, 7> centralStacksIndices() const;

		// Returns the indices for EndStacks
		std::array<size_t, 4> endStacksIndices() const;

		// Updated the game state if the end stacks are complete
		void checkWin();

		void reset(Game&& other);

	private:
		// Returns the open stack
		CardStack& openStack();

		// Returns the closed stack
		CardStack& closedStack();

		// Moves stack to central stack
		bool canMoveToCentralStack(CardStack& sourceStack, size_t sourceCardIndex, CardStack& destStack);
		bool canMoveToEndStack(CardStack& sourceStack, size_t sourceCardIndex, CardStack& destStack);

		std::vector<CardStack> m_stacks;
		State m_state = State::Playing;
	};
}