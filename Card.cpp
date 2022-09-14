#include "Card.h"

namespace panda
{
	Card::Card(int number, Suit suit, State state)
		: m_number(number)
		, m_suit(suit)
		, m_state(state)
	{
	}

	void Card::flip()
	{
		if (m_state == State::Open)
			m_state = State::Closed;
		else if (m_state == State::Closed)
			m_state = State::Open;
	}
}