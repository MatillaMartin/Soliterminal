#pragma once

#include "Console.h"

#include <cmath>
#include <optional>

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
		Render(const Game& game, const GameControl& control, const GameLayout& layout);

		// Updates the rendering output
		void update();

	private:
		int m_cardWidth = 4;           // spaces per card width, for card like 10
		int m_cardHeight = 3;          // spaces per card height
		int m_stackSpacing = 4;        // space between stacks
		int m_cardSpacing = 0;         // space between cards
		int m_cardSpreadHeight = 1;    // spaces per card height for spread cards

		int m_openColorFg = 0xF;
		int m_closedColorFg = 0xF;
		int m_closedColorBg = 0x3;
		int m_emptyColorFg = 0xF;
		int m_selectColor = 0xF;
		int m_markColor = 0xA;
		int m_clearColor = 0x0;

		// Returns the console position for each layout element
		std::optional<std::pair<int, int>> layoutToConsole(int index);

		int cardCenterX() const { return static_cast<int>(std::floor((m_cardWidth - 1) / 2.0)); }
		int cardCenterY() const { return static_cast<int>(std::floor((m_cardHeight - 1) / 2.0)); }

		void renderStacks();
		void renderControlSelect();
		void renderControlMark();

		void drawCardSpread(const Card& card, int row, int column);
		void drawCard(const Card& card, int row, int column);
		void drawEmpty(int x, int y);
		void drawEmpty(char text, int x, int y);
		void drawEmptyClosedStack(int x, int y);
		void drawControlSelect(int x, int y);
		void drawControlMark(int x, int y);
		void drawControl(int x, int y);

		void drawShade(int x, int y);

		const Game& m_game;
		const GameControl& m_control;
		const GameLayout& m_layout;

		Console m_console;
	};
}