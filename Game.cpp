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
		std::optional<CardStack> top = m_stacks.openStack.takeTop();
		if (top)
		{
			m_stacks.openStack.append(std::move(*top));
		}
	}

	void Game::resetOpenClosedStack()
	{
		assert(m_stacks.openStack.size() == 0);
		if (m_stacks.openStack.size() != 0)
			return;

		// flip stack
		std::swap(m_stacks.openStack, m_stacks.closedStack);
		// swap open and closed stack
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