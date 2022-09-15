#pragma once

#include "GameAction.h"

#include <utility>

namespace panda
{
	class CardStack;
	class GameLayout;
	class Game;

	class GameControl
	{
	public:
		GameControl(Game & game, GameLayout& layout);

		void action(GameAction action);

		int stackIndex() const;
		int cardIndex() const;

	private:
		void moveCursor(int dx, int dy);
		const CardStack& stack();
		bool isCentralStack();

		Game& m_game;
		const GameLayout& m_layout;
		int m_cardIndex = 0;
		int m_stackIndex = 0;
	};
}