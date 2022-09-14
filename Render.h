#pragma once

#include "Console.h"
#include <cmath>

namespace panda
{
	class Game;
	struct Card;
	class Stack;

	class Render
	{
	public:
		Render(const Game& game);

		// Updates the rendering output
		void update();

	private:
		int m_cardWidth = 4;       // spaces per card width, for card like 10
		int m_cardHeight = 3;      // spaces per card height
		int m_stackSpacing = 4;    // space between stacks
		int m_cardSpacing = 0;     // space between cards

		int cardCenterX() const { return std::floor((m_cardWidth - 1) / 2.0); }
		int cardCenterY() const { return std::floor((m_cardHeight - 1) / 2.0); }

		void drawCardStacked(const Card& card, int row, int column) const;
		void drawCard(const Card& card, int row, int column) const;
		void drawEmpty(int x, int y) const;

		const Game& m_game;
		Console m_console;
	};
}