#include "GameControl.h"

#include "CardStack.h"
#include "Game.h"
#include "GameLayout.h"

namespace panda
{
	const CardStack& GameControl::stack()
	{
		const CardStack& stack = m_layout.stacks()[m_stackIndex];
		return stack;
	}

	GameControl::GameControl(Game& game, GameLayout& gameLayout)
		: m_game(game)
		, m_layout(gameLayout)
	{
		// start at last stack
		m_stackIndex = static_cast<int>(m_layout.stacks().size()) - 1;
	}

	void GameControl::action(GameAction action)
	{
		auto isCentralStack = [this]() -> bool { return m_game.isCentralStack(m_stackIndex); };

		// apply action map to graph
		if (action == GameAction::Up)
		{
			if (isCentralStack())
			{
				if (m_cardIndex == 0)
				{
					// move up a stack
					m_stackIndex = m_layout.up(m_stackIndex);
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
				// move down the cards until last
				if (m_cardIndex < stack().size() - 1)
					m_cardIndex++;
			}
			else
			{
				m_stackIndex = m_layout.down(m_stackIndex);
			}
		}
		else if (action == GameAction::Left)
		{
			// move to the left
			m_stackIndex = m_layout.left(m_stackIndex);
			if (isCentralStack())
			{
				// update the cardIndex for the new stack
				m_cardIndex = std::min(m_cardIndex, stack().size() - 1);
			}
		}
		else if (action == GameAction::Right)
		{
			// move to the right
			m_stackIndex = m_layout.right(m_stackIndex);
			if (isCentralStack())
			{
				// update the cardIndex for the new stack
				m_cardIndex = std::min(m_cardIndex, stack().size() - 1);
			}
		}
		else if (action == GameAction::Use)
		{
			if (m_state == State::Select)
			{
				if (m_game.isClosedStack(m_stackIndex))
				{
					m_game.openCard();
				}
				else
				{
					m_markedStackIndex = m_stackIndex;
					m_markedCardIndex = m_cardIndex;
					m_state = State::Move;
				}
			}
			else if(m_state == State::Move)
			{
				bool moved = m_game.moveCards(m_markedStackIndex, m_markedCardIndex, m_stackIndex);
				if (moved)
				{
					m_state = State::Select;
					m_markedStackIndex = 0;
					m_markedCardIndex = 0;
				}
			}
		}
	}

	int GameControl::stackIndex() const { return m_stackIndex; }

	int GameControl::cardIndex() const { return m_cardIndex; }
}