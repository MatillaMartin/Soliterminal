#pragma once

namespace panda
{
	struct Card
	{
		enum class State
		{
			Open,
			Closed
		};
		enum class Suit
		{
			Club,
			Diamond,
			Heart,
			Spade
		};

		Card(int number, Suit suit, State state);

		void flip();

		int m_number = 0;
		Suit m_suit = Suit::Club;
		State m_state = State::Open;
	};
}