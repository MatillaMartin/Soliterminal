#pragma once

#include "Console.h"
#include "Render.h"

#include <cmath>
#include <optional>
namespace panda
{
	class Game;
	class GameSelection;
	class Layout;
	struct Card;
	class Stack;

	typedef std::pair<int, int> vec2i;

	class GameRender : public Render
	{
	public:
		GameRender(const Game& game, const GameSelection& selection, const Layout& layout, Console& console);

		// Updates the rendering output
		void update();

	private:
		int m_cardWidth = 4;           // spaces per card width, for card like 10
		int m_cardHeight = 3;          // spaces per card height
		int m_stackSpacing = 4;        // space between stacks
		int m_cardSpreadHeight = 1;    // spaces per card height for spread cards

		int m_openColorFg = 0xF;
		int m_closedColorFg = 0xF;
		int m_closedColorBg = 0x3;
		int m_emptyColorFg = 0xF;
		int m_selectColor = 0xF;
		int m_markColor = 0xA;
		int m_clearColor = 0x0;

		// Returns the console position for each layout element
		std::optional<std::pair<int, int>> indexToConsole(size_t index);

		int cardCenterX() const { return static_cast<int>(std::floor((m_cardWidth - 1) / 2.0)); }
		int cardCenterY() const { return static_cast<int>(std::floor((m_cardHeight - 1) / 2.0)); }

		void renderStacks();
		void renderControlSelect();
		void renderControlMark();

		std::optional<vec2i> position(size_t stackIndex, size_t cardIndex);

		void drawCardSpread(const Card& card, vec2i pos);
		void drawCard(const Card& card, vec2i pos);
		void drawEmpty(vec2i pos);
		void drawEmpty(char text, vec2i pos);
		void drawEmptyClosedStack(vec2i pos);
		void drawControlSelect(vec2i pos);
		void drawControlMark(vec2i pos);
		void drawControl(vec2i pos);
		void drawShade(int x, int y);

		const Game& m_game;
		const GameSelection& m_selection;
		const Layout& m_layout;
		Console& m_console;
	};
}