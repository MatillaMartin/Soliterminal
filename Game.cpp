#include "Game.h"

#include <assert.h>

namespace panda
{
	Game::Game(Stacks&& stacks)
	{
		m_stacks.insert(m_stacks.end(), std::make_move_iterator(stacks.endStack.begin()), std::make_move_iterator(stacks.endStack.end()));
		m_stacks.insert(m_stacks.end(), std::move(stacks.openStack));
		m_stacks.insert(m_stacks.end(), std::move(stacks.closedStack));
		m_stacks.insert(m_stacks.end(), std::make_move_iterator(stacks.centralStack.begin()), std::make_move_iterator(stacks.centralStack.end()));
	}

	void Game::openCard()
	{
		std::optional<CardStack> top = m_stacks[closedStackIndex()].takeTop();
		if (top)
		{
			m_stacks[openStackIndex()].append(std::move(*top));
		}
		else
		{
			resetClosedStack();
		}
	}

	void Game::resetClosedStack()
	{
		assert(m_stacks[closedStackIndex()].size() == 0);
		if (m_stacks[closedStackIndex()].size() != 0)
			return;

		if (m_stacks[openStackIndex()].size() == 0)
			return;

		// swap open and closed stack
		std::swap(m_stacks[openStackIndex()], m_stacks[closedStackIndex()]);
		// invert stack
		m_stacks[closedStackIndex()].invertOrder();
		// flip stack
		m_stacks[closedStackIndex()].flipAll();
	}

	bool Game::moveCards(int originStack, int cardOriginIndex, int destStack)
	{
		if (originStack >= m_stacks.size() || originStack < 0)
			return false;
		if (destStack >= m_stacks.size() || destStack < 0)
			return false;

		std::optional<CardStack> toMove = m_stacks[originStack].take(cardOriginIndex);
		if (!toMove)
			return false;

		bool ok = m_stacks[destStack].append(std::move(*toMove));
		return ok;
	}

	bool Game::isEndStack(int index) const { return index >= 0 && index < 4; }

	bool Game::isCentralStack(int index) const { return index >= 6 && index < 12; }

	bool Game::isOpenStack(int index) const { return index == openStackIndex(); }

	bool Game::isClosedStack(int index) const { return index == closedStackIndex(); }

	int Game::openStackIndex() const { return 4; };

	int Game::closedStackIndex() const { return 5; }

	std::vector<int> Game::endStacksIndices() const { return {0, 1, 2, 3}; }

	std::vector<int> Game::centralStacksIndices() const { return {6, 7, 8, 9, 10, 11}; }
}