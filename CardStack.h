#pragma once
#include <list>
#include <optional>
#include <vector>

namespace panda
{
	class Card;

	class CardStack
	{
	public:
		CardStack(const std::vector<Card>& cards);

		// Takes all cards after index, including index
		// Returns empty optional if no cards can be taken
		std::optional<CardStack> take(int index);

		// Takes top card of stack
		// Returns empty optional if no cards can be taken
		std::optional<CardStack> takeTop();

		// Appends stack at the end of the current stack
		// Returns false if operation fails
		bool append(CardStack&& stack);

		// Flips all cards in the stack
		void flipAll();

		// Flips the top card in the stack
		void flipTop();

		// Number of cards in stack
		int size();

	private:
		std::list<Card> m_cards;
	};
}