#include "Card.h"

#include <cmath>

namespace panda
{
	Card::Card(int number, Suit suit, State state)
		: number(number)
		, suit(suit)
		, state(state)
	{
	}

	void Card::flip()
	{
		if (state == State::Open)
			state = State::Closed;
		else if (state == State::Closed)
			state = State::Open;
	}

	bool Card::isSameColor(const Card& other) const
	{
		auto isBlack = [](const Card& card) { return card.suit == Suit::Spade || card.suit == Suit::Club; };
		bool thisBlack = isBlack(*this);
		bool otherBlack = isBlack(other);

		if (thisBlack && otherBlack)
			return true;

		if (!thisBlack && !otherBlack)
			return true;

		return false;
	}

	bool Card::isSameSuit(const Card& other) const { return this->suit == other.suit; }

	bool Card::isLower(const Card& other) const { return this->number < other.number; }

	bool Card::isHigher(const Card& other) const { return this->number > other.number; }

	bool Card::isAdjacent(const Card& other) const { return std::abs(this->number - other.number) == 1; }
}