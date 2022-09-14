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
}