#pragma once
#include <algorithm>

namespace panda
{
	struct Card
	{
		enum class State
		{
			Open = 0,
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
		
		bool isSameSuit(const Card& other) const;
		bool isSameColor(const Card& other) const;
		bool isLower(const Card& other) const;
		bool isHigher(const Card& other) const;
		bool isAdjacent(const Card& other) const;

		int number = 0;
		Suit suit = Suit::Club;
		State state = State::Open;
	};
}