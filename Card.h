#pragma once
#include <algorithm>

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
			Heart,
			Diamond,
			Club,
			Spade
		};


		Card(int number = 0, Suit suit = Suit::Club, State state = State::Open);

		void flip();

		bool isSameColor(const Card& other);
		bool isLower(const Card& other);

		int number = 0;
		Suit suit = Suit::Club;
		State state = State::Open;
	};
}