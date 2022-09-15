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

		Card(int number = 0, Suit suit = Suit::Club, State state = State::Open);

		void flip();

		int number = 0;
		Suit suit = Suit::Club;
		State state = State::Open;
	};
}