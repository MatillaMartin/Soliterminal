#pragma once

#include "Console.h"
#include <cmath>

namespace panda
{
	class Game;
	class GameControl;
	class GameLayout;
	struct Card;
	class Stack;

	class Render
	{
	public:
		Render(const GameControl & control, const GameLayout & layout);

		// Updates the rendering output
		void update();

	private:
		int m_cardWidth = 4;       // spaces per card width, for card like 10
		int m_cardHeight = 3;      // spaces per card height
		int m_stackSpacing = 4;    // space between stacks
		int m_cardSpacing = 0;     // space between cards

		// Returns if the card stack should be spread
		bool isSpread(int x, int y);

		// Returns the grid position for each layout element
		std::pair<int, int> position(int x, int y);

		int cardCenterX() const { return static_cast<int>(std::floor((m_cardWidth - 1) / 2.0)); }
		int cardCenterY() const { return static_cast<int>(std::floor((m_cardHeight - 1) / 2.0)); }

		void drawCardStacked(const Card& card, int row, int column) const;
		void drawCard(const Card& card, int row, int column) const;
		void drawEmpty(int x, int y) const;

		const GameControl& m_control;
		const GameLayout& m_layout;

		Console m_console;
	};
}