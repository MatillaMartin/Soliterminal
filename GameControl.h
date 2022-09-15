#pragma once

#include "GameAction.h"
#include "GameLayout.h"

namespace panda
{
	class CardStack;

	class GameControl
	{
	public:
		GameControl(GameLayout & layout);

		void action(GameAction action);

		std::pair<int, int> stackIndex() const;
		int cardIndex() const;

	private:
		void moveCursor(int dx, int dy);
		const CardStack& stack();
		bool isCentralStack();

		const StackTable& m_table;
		int m_cardIndex = 0;
		int m_stackX = 0;
		int m_stackY = 0;
		int m_tableWidth = 6;
		int m_tableHeight = 2;
	};
}