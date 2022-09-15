#include "GameControl.h"

#include "CardStack.h"
#include "GameLayout.h"

#include <algorithm>

namespace panda
{
	void GameControl::moveCursor(int dx, int dy)
	{
		int nextX = m_stackX + dx;
		int nextY = m_stackY + dy;
		m_stackX = std::clamp(nextX, 0, m_tableWidth - 1);
		m_stackY = std::clamp(nextY, 0, m_tableHeight - 1);
	}

	const CardStack& GameControl::stack() 
	{
		const CardStack& stack = m_table[m_stackX][m_stackY]();
		return stack; 
	}

	bool GameControl::isCentralStack() { return m_stackY == 1; }

	GameControl::GameControl(GameLayout& gameLayout)
		: m_table(gameLayout.table())
	{
		// start top left
		m_stackX = 5;
		m_stackY = 0;
	}

	void GameControl::action(GameAction action)
	{
		// apply action map to graph
		if (action == GameAction::Up)
		{
			if (isCentralStack())
			{
				if (m_cardIndex == 0)
				{
					// move up a stack
					moveCursor(0, -1);
				}
				else
				{
					// move up the cards
					m_cardIndex--;
				}
			}
		}
		else if (action == GameAction::Down)
		{
			const CardStack& current = stack();
			if (isCentralStack())
			{
				if (m_cardIndex == 0)
				{
					// move down a stack
					moveCursor(0, 1);
				}
				else
				{
					// move down the cards until last
					if (m_cardIndex < stack().size() - 1)
						m_cardIndex++;
				}
			}
		}
		else if (action == GameAction::Left)
		{
			// move to the left
			moveCursor(-1, 0);
			if (isCentralStack())
			{
				// update the cardIndex for the new stack
				m_cardIndex = std::max(m_cardIndex, stack().size() - 1);
			}
		}
		else if (action == GameAction::Right)
		{
			// move to the right
			moveCursor(1, 0);
			if (isCentralStack())
			{
				// update the cardIndex for the new stack
				m_cardIndex = std::max(m_cardIndex, stack().size() - 1);
			}
		}
		else if (action == GameAction::Use)
		{
			// nothing here yet
		}
	}
	
	std::pair<int, int> GameControl::stackIndex() const { return {m_stackX, m_stackY}; }
	
	int GameControl::cardIndex() const { return m_cardIndex; }
}