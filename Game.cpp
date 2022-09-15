#include "Game.h"

#include <assert.h>

namespace panda
{
	Game::Game(Stacks state)
		: m_stacks(std::move(state))
	{
	}

	const Game::Stacks& Game::stacks() const { return m_stacks; }

	void Game::openCard()
	{
		std::optional<CardStack> top = m_stacks.closedStack.takeTop();
		if (top)
		{
			m_stacks.openStack.append(std::move(*top));
		}
		else
		{
			resetClosedStack();
		}
	}

	void Game::resetClosedStack()
	{
		assert(m_stacks.closedStack.size() == 0);
		if (m_stacks.closedStack.size() != 0)
			return;

		if (m_stacks.openStack.size() == 0)
			return;

		// swap open and closed stack
		std::swap(m_stacks.openStack, m_stacks.closedStack);
		// invert stack
		m_stacks.closedStack.invertOrder();
		// flip stack
		m_stacks.closedStack.flipAll();
	}

	bool Game::moveCards(CardStack& origin, int cardOriginIndex, CardStack& dest)
	{
		std::optional<CardStack> toMove = origin.take(cardOriginIndex);
		if (!toMove)
			return false;

		bool ok = dest.append(std::move(*toMove));
		return ok;
	}
}