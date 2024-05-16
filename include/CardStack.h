#pragma once
#include "Card.h"

#include <optional>
#include <vector>

namespace panda
{
	class CardStack
	{
	public:
		explicit CardStack(std::vector<Card>&& cards = {});

		// Takes all cards after index, including index
		// Returns empty optional if no cards can be taken
		std::optional<CardStack> take(size_t index);

		// Takes top card of stack
		// Returns empty optional if no cards can be taken
		std::optional<CardStack> takeTop();

		// Returns the card at the bottom of the stack, under the rest of the cards
		std::optional<Card> bottom() const;

		// Returns the card at the top, first card to be visible
		std::optional<Card> top() const;

		// Returns the index of the card at the top, first card to be visible
		size_t topIndex() const;

		// Returns the index of the first open card, if any
		std::optional<size_t> firstOpenCard() const;

		// Returns all the cards in the stack
		const std::vector<Card>& cards() const;

		// Appends stack at the end of the current stack
		// Returns false if operation fails
		bool append(CardStack&& stack);

		// Inverts the order of the stack
		void invertOrder();

		// Flips all cards in the stack
		void flipAll();

		// Flips the top card in the stack
		void flipTop();

		// Number of cards in stack
		size_t size() const;

	private:
		std::vector<Card> m_cards;
	};
}