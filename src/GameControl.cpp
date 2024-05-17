#include "GameControl.h"

#include "CardStack.h"
#include "Game.h"
#include "Layout.h"

#include <iostream>
namespace panda
{
	const CardStack& GameControl::stack()
	{
		const CardStack& stack = m_game.stacks()[m_sel.stackIndex];
		return stack;
	}

	GameControl::GameControl(Game& game, Layout& gameLayout)
		: m_game(game)
		, m_layout(gameLayout)
	{
	}

	void GameControl::reset()
	{
		m_sel.reset();
	}

	bool GameControl::isCentralStack()
	{
		return m_game.isCentralStack(m_sel.stackIndex);
	}

	void GameControl::changeStack(size_t stackIndex)
	{
		bool wasCentral = isCentralStack();
		m_sel.stackIndex = stackIndex;
		changeCard(m_sel.cardIndex);    // update using last card index, try to keep it
	}

	bool GameControl::changeCard(size_t cardIndex)
	{
		size_t stackLastCardIndex = std::max(static_cast<size_t>(0), stack().size() - 1);    // index of available cardIndex or zero

		// update the cardIndex for the new stack
		if (isCentralStack())
		{
			if (m_sel.stackIndex < 0 || m_sel.stackIndex >= m_game.stacks().size())
				return false;

			// If card index over our stack count, point to last
			if (cardIndex > stackLastCardIndex)
			{
				m_sel.cardIndex = stackLastCardIndex;
				return false;
			}

			// if there are any open cards, make sure to point to the first open card only
			std::optional<size_t> firstOpenCardIndex = m_game.stacks()[m_sel.stackIndex].firstOpenCard();
			if (!firstOpenCardIndex)
			{
				// if all the cards are closed, select last card in pile
				m_sel.cardIndex = stackLastCardIndex;
				return false;
			}

			// if we are trying to access before the last open one, do not allow
			if (cardIndex < *firstOpenCardIndex)
			{
				m_sel.cardIndex = *firstOpenCardIndex;    // select a non flipped card
				return false;
			}
		}
		else
		{
			m_sel.cardIndex = stackLastCardIndex;    // for compacted stacks, pick top card index
			return false;
		}

		m_sel.cardIndex = cardIndex;
		return true;
	}

	void GameControl::action(const Action& action)
	{
		if (action == Action::Up)
		{
			if (isCentralStack())
			{
				if (m_sel.cardIndex == 0)
				{
					// move up a stack
					changeStack(m_layout.up(m_sel.stackIndex));
				}
				else
				{
					// move up the cards
					bool accepted = changeCard(m_sel.cardIndex - 1);
					if (!accepted)
					{
						changeStack(m_layout.up(m_sel.stackIndex));
					}
				}
			}
		}
		else if (action == Action::Down)
		{
			const CardStack& current = stack();
			if (isCentralStack())
			{
				// move down the cards until last
				bool accepted = changeCard(m_sel.cardIndex + 1);
				if (!accepted)
				{
					changeStack(m_layout.down(m_sel.stackIndex));
				}
			}
			else
			{
				changeStack(m_layout.down(m_sel.stackIndex));
			}
		}
		else if (action == Action::Left)
		{
			// move to the left
			changeStack(m_layout.left(m_sel.stackIndex));
		}
		else if (action == Action::Right)
		{
			// move to the right
			changeStack(m_layout.right(m_sel.stackIndex));
		}
		else if (action == Action::Use)
		{
			if (m_sel.state == GameSelection::State::Select)
			{
				if (m_game.isClosedStack(m_sel.stackIndex))
				{
					m_game.openCard();
				}
				else if (m_game.isFlippedCard(m_sel.stackIndex, m_sel.cardIndex))
				{
					m_game.flipCard(m_sel.stackIndex, m_sel.cardIndex);
				}
				else
				{
					if (m_game.stacks()[m_sel.stackIndex].size() == 0)
						return;

					m_sel.markedStackIndex = m_sel.stackIndex;
					m_sel.markedCardIndex = m_sel.cardIndex;
					m_sel.state = GameSelection::State::Move;
				}
			}
			else if (m_sel.state == GameSelection::State::Move)
			{
				m_game.moveCards(m_sel.markedStackIndex, m_sel.markedCardIndex, m_sel.stackIndex);
				m_sel.state = GameSelection::State::Select;
				m_sel.markedStackIndex = 0;
				m_sel.markedCardIndex = 0;
			}
		}

		// check if there is a win after every action
		m_game.checkWin();
	}

	const GameSelection& GameControl::selection() const
	{
		return m_sel;
	}
}