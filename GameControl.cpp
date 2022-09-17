#include "GameControl.h"

#include "CardStack.h"
#include "Game.h"
#include "GameLayout.h"

namespace panda
{
	const CardStack& GameControl::stack()
	{
		const CardStack& stack = m_game.stacks()[m_stackIndex];
		return stack;
	}

	GameControl::GameControl(Game& game, GameLayout& gameLayout)
		: m_game(game)
		, m_layout(gameLayout)
	{
		// start at last stack
		m_stackIndex = static_cast<int>(m_game.stacks().size()) - 1;
	}

	bool GameControl::isCentralStack() { return m_game.isCentralStack(m_stackIndex); }

	void GameControl::changeStack(int stackIndex) 
	{
		bool wasCentral = isCentralStack();
		m_stackIndex = stackIndex;
		changeCard(m_cardIndex);    // update using last card index, try to keep it
	}

	void GameControl::changeCard(int cardIndex)
	{
		int stackLastCardIndex = std::max(0, stack().size() - 1);    // index of available cardIndex or zero

		m_cardIndex = cardIndex;
		// update the cardIndex for the new stack
		if (isCentralStack())
		{
			if (m_stackIndex < 0 || m_stackIndex >= m_game.stacks().size())
				return;

			// If try to keep index, do not set to zero if possible
			m_cardIndex = std::min(m_cardIndex, stackLastCardIndex);

			std::optional<int> firstOpenCardIndex = m_game.stacks()[m_stackIndex].firstOpenCard();
			if (!firstOpenCardIndex)
			{
				// if all the cards are flipped, select last card in pile
				m_cardIndex = stackLastCardIndex;
			}
			else
			{
				// otherwise, select the first open card
				m_cardIndex = std::max(m_cardIndex, *firstOpenCardIndex);    // select a non flipped card
			}
		}
		else
		{
			m_cardIndex = stackLastCardIndex;    // for compacted stacks, pick top card index
		}
	}

	void GameControl::action(GameAction action)
	{
		if (action == GameAction::Up)
		{
			if (isCentralStack())
			{
				if (m_cardIndex == 0)
				{
					// move up a stack
					changeStack(m_layout.up(m_stackIndex));
				}
				else
				{
					// move up the cards
					changeCard(m_cardIndex - 1);
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
				{
					changeCard(m_cardIndex + 1);
				}
			}
			else
			{
				changeStack(m_layout.down(m_stackIndex));
			}
		}
		else if (action == GameAction::Left)
		{
			// move to the left
			changeStack(m_layout.left(m_stackIndex));
		}
		else if (action == GameAction::Right)
		{
			// move to the right
			changeStack(m_layout.right(m_stackIndex));
		}
		else if (action == GameAction::Use)
		{
			if (m_state == State::Select)
			{
				if (m_game.isClosedStack(m_stackIndex))
				{
					m_game.openCard();
				}
				else if (m_game.isFlippedCard(m_stackIndex, m_cardIndex))
				{
					m_game.flipCard(m_stackIndex, m_cardIndex);
				}
				else
				{
					m_markedStackIndex = m_stackIndex;
					m_markedCardIndex = m_cardIndex;
					m_state = State::Move;
				}
			}
			else if (m_state == State::Move)
			{
				bool moved = m_game.moveCards(m_markedStackIndex, m_markedCardIndex, m_stackIndex);
				m_state = State::Select;
				m_markedStackIndex = 0;
				m_markedCardIndex = 0;
			}
		}
	}

	int GameControl::stackIndex() const { return m_stackIndex; }

	int GameControl::cardIndex() const { return m_cardIndex; }

	int GameControl::markedStackIndex() const { return m_markedStackIndex; }

	int GameControl::markedCardIndex() const { return m_markedCardIndex; }

	GameControl::State GameControl::state() const { return m_state; }
}