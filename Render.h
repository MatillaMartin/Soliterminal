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
		Render(const Game & game, const GameControl & control, const GameLayout & layout);

		// Updates the rendering output
		void update();

	private:
		int m_cardWidth = 4;       // spaces per card width, for card like 10
		int m_cardHeight = 3;      // spaces per card height
		int m_stackSpacing = 4;    // space between stacks
		int m_cardSpacing = 0;     // space between cards

		// Returns the console position for each layout element
		std::pair<int, int> layoutToConsole(int index);

		int cardCenterX() const { return static_cast<int>(std::floor((m_cardWidth - 1) / 2.0)); }
		int cardCenterY() const { return static_cast<int>(std::floor((m_cardHeight - 1) / 2.0)); }

		void drawCardSpread(const Card& card, int row, int column) const;
		void drawCard(const Card& card, int row, int column) const;
		void drawEmpty(int x, int y) const;
		void drawControlSelect(int x, int y) const;
		void drawControlMark(int x, int y) const;

		const Game& m_game;
		const GameControl& m_control;
		const GameLayout& m_layout;

		Console m_console;
	};
}