#pragma once

#include "Console.h"

namespace panda
{
	class Game;
	struct Card;
	class Stack;

	class Render
	{
	public:
		Render(const Game& game);
		void update();

	private:
		void drawCard(const Card& card, int row, int column) const;
		void drawEmpty(int row, int column) const;

		const Game& m_game;
		Console m_console;
	};
}