#pragma once

#include "GameAction.h"

#include <utility>

namespace panda
{
	class CardStack;
	class GameLayout;
	class GameControl
	{
	public:
		GameControl(GameLayout& layout);

		void action(GameAction action);

		std::pair<int, int> stackIndex() const;
		int cardIndex() const;

	private:
		void moveCursor(int dx, int dy);
		const CardStack& stack();
		bool isCentralStack();

		const GameLayout& m_layout;
		int m_cardIndex = 0;
		int m_stackX = 0;
		int m_stackY = 0;
		int m_tableWidth = 0;
		int m_tableHeight = 0;
	};
}