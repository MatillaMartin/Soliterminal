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

	bool Game::moveCards(int originStackIndex, int cardOriginIndex, int destStackIndex)
	{
		// check if cards are compatible for appending
		auto canAppend = [](Card& source, CardStack& dest) -> bool { 
			std::optional<Card> destBot = dest.bottom(); 

			if (destBot)
			{
				return !destBot->isSameColor(source) && source.isLower(*destBot);
			}
			if (!destBot)
				return true;

			return false;
		};

		if (originStackIndex >= m_stacks.size() || originStackIndex < 0)
			return false;
		if (destStackIndex >= m_stacks.size() || destStackIndex < 0)
			return false;
		if (originStackIndex == destStackIndex)
			return false;

		CardStack& destStack = m_stacks[destStackIndex];

		if (isCentralStack(destStackIndex))
		{
			std::optional<Card> cardOrigin = m_stacks[originStackIndex].at(cardOriginIndex);
			if (!cardOrigin)
				return false;

			if (canAppend(*cardOrigin, destStack))
			{
				std::optional<CardStack> toMove = m_stacks[originStackIndex].take(cardOriginIndex);
				if (!toMove)
					return false;

				bool ok = destStack.append(std::move(*toMove));
				return ok;
			}
		}

		return false;
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