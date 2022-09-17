#include "Game.h"

#include <assert.h>

namespace panda
{
	Game::Game(Stacks&& stacks)
	{
		m_stacks.insert(m_stacks.end(), std::move(stacks.closedStack));
		m_stacks.insert(m_stacks.end(), std::move(stacks.openStack));
		m_stacks.insert(m_stacks.end(), std::make_move_iterator(stacks.endStack.begin()), std::make_move_iterator(stacks.endStack.end()));
		m_stacks.insert(m_stacks.end(), std::make_move_iterator(stacks.centralStack.begin()), std::make_move_iterator(stacks.centralStack.end()));
	}

	void Game::openCard()
	{
		std::optional<CardStack> top = closedStack().takeTop();
		if (top)
		{
			top->flipAll();
			openStack().append(std::move(*top));
		}
		else
		{
			resetClosedStack();
		}
	}

	void Game::resetClosedStack()
	{
		assert(closedStack().size() == 0);
		if (closedStack().size() != 0)
			return;

		if (openStack().size() == 0)
			return;

		// swap open and closed stack
		std::swap(openStack(), closedStack());
		// invert stack
		closedStack().invertOrder();
		// flip stack
		closedStack().flipAll();
	}

	bool Game::moveCards(int sourceStackIndex, int sourceCardIndex, int destStackIndex)
	{
		// check stack indices are inside bounds
		if (sourceStackIndex >= m_stacks.size() || sourceStackIndex < 0)
			return false;
		if (destStackIndex >= m_stacks.size() || destStackIndex < 0)
			return false;

		// if stack indices are the same, no moving can happen
		if (sourceStackIndex == destStackIndex)
			return false;

		CardStack& destStack = m_stacks[destStackIndex];
		CardStack& sourceStack = m_stacks[sourceStackIndex];

		// check card index is inside bounds
		if (sourceCardIndex >= sourceStack.size() || sourceCardIndex < 0)
			return false;

		// check rules to move between stacks
		if (isCentralStack(destStackIndex))
		{
			if (!canMoveToCentralStack(sourceStack, sourceCardIndex, destStack))
				return false;
		}
		else if (isEndStack(destStackIndex))
		{
			if (!canMoveToEndStack(sourceStack, sourceCardIndex, destStack))
				return false;
		}
		else
		{
			// can't apply move operations on any other destination stacks
			return false;
		}

		// take from source stack and move to dest stack
		std::optional<CardStack> toMove = sourceStack.take(sourceCardIndex);
		if (!toMove)
			return false;

		bool ok = destStack.append(std::move(*toMove));

		// check it the user has won
		checkWin();

		return ok;
	}

	bool Game::isFlippedCard(int stack, int cardIndex)
	{
		if (stack >= m_stacks.size() || stack < 0)
			return false;

		CardStack& sourceStack = m_stacks[stack];
		if (cardIndex >= sourceStack.size() || cardIndex < 0)
			return false;

		const Card& card = sourceStack.cards()[cardIndex];
		return card.state == Card::State::Closed;
	}

	bool Game::flipCard(int stack, int cardIndex)
	{
		if (stack >= m_stacks.size() || stack < 0)
			return false;

		CardStack& sourceStack = m_stacks[stack];
		if (cardIndex >= sourceStack.size() || cardIndex < 0)
			return false;

		// has to be the top card in the stack
		int stackTopIndex = std::max(0, sourceStack.size() - 1);
		if (cardIndex != stackTopIndex)
			return false;

		sourceStack.flipTop();
		return true;
	}

	bool Game::isEndStack(int index) const { return index >= 2 && index < 6; }

	bool Game::isCentralStack(int index) const { return index >= 6 && index < 13; }

	bool Game::isOpenStack(int index) const { return index == 1; }

	bool Game::isClosedStack(int index) const { return index == 0; }

	CardStack& Game::openStack() { return m_stacks[1];  }

	CardStack& Game::closedStack() { return m_stacks[0]; };

	std::array<int, 7> Game::centralStacksIndices() const { return {6, 7, 8, 9, 10, 11, 12}; }

	std::array<int, 4> Game::endStacksIndices() const { return {0, 1, 2, 3}; }

	void Game::checkWin()
	{
		bool stacksComplete = true;
		for (auto stackIndex : endStacksIndices())
		{
			const CardStack& stack = m_stacks[stackIndex];
			if (stack.size() != 13)
				return;
			if (stack.size() == 0)
				return;

			// iterate top to bottom
			for (auto it = stack.cards().begin(), end_it = std::prev(stack.cards().end()); it != end_it; ++it)
			{
				bool adjacent = std::next(it)->isAdjacent(*it);
				bool increasing = std::next(it)->isHigher(*it);
				bool sameSuit = std::next(it)->isSameSuit(*it);
				if (!adjacent || !increasing || !sameSuit)
					return;
			}

			stacksComplete &= true;
		}

		if (stacksComplete)
			m_state = State::Win;
	}

	bool Game::canMoveToCentralStack(CardStack& sourceStack, int sourceCardIndex, CardStack& destStack)
	{
		// Card to be moved in
		const Card& sourceCard = sourceStack.cards()[sourceCardIndex];

		std::optional<Card> destTop = destStack.top();
		if (destTop)
		{
			// if dest central stack has cards, source has to be compatible
			return !sourceCard.isSameColor(*destTop) && sourceCard.isLower(*destTop) && sourceCard.isAdjacent(*destTop);
		}

		// if the dest central stack is empty, it can be moved in
		return true;
	}

	bool Game::canMoveToEndStack(CardStack& sourceStack, int sourceCardIndex, CardStack& destStack)
	{
		// has to be the top card in the stack
		int stackTopIndex = std::max(0, sourceStack.size() - 1);
		if (sourceCardIndex != stackTopIndex)
			return false;

		// Card to be moved in
		const Card& sourceCard = sourceStack.cards()[sourceCardIndex];
		std::optional<Card> destTop = destStack.top();
		if (destTop)
		{
			// If end stack has any cards already, source has to be compatible
			return sourceCard.isSameSuit(*destTop) && sourceCard.isHigher(*destTop) && sourceCard.isAdjacent(*destTop);
		}

		// If no end stack has no cards, the source card has to be an ace
		return sourceCard.number == 1;
	}

	void Game::reset(Game&& other) 
	{ 
		*this = other;
	}
}