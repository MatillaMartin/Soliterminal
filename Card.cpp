#include "Card.h"

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

	bool Card::isSameColor(const Card& other)
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

	bool Card::isLower(const Card& other)
	{ 
		return this->number < other.number;
	}
}