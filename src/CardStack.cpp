#include "CardStack.h"

#include "Card.h"

#include <iterator>

namespace panda
{
	CardStack::CardStack(std::vector<Card>&& cards)
		: m_cards(cards)
	{
	}

	std::optional<CardStack> CardStack::take(size_t index)
	{
		if (index >= m_cards.size())
			return {};

		std::vector<Card> out;
		auto first = m_cards.begin();
		std::advance(first, index);
		std::move(first, m_cards.end(), std::back_inserter(out));

		// erase cards from original stack
		m_cards.resize(index);
		return std::optional<CardStack>(std::move(out));
	}

	std::optional<CardStack> CardStack::takeTop()
	{
		if (m_cards.empty())
			return {};

		return take(m_cards.size() - 1);
	}

	std::optional<Card> CardStack::top() const
	{
		if (m_cards.empty())
			return {};

		return m_cards.back();
	}

	size_t CardStack::topIndex() const { return m_cards.size() - 1; }

	std::optional<Card> CardStack::bottom() const
	{
		if (m_cards.empty())
			return {};

		return m_cards.front();
	}

	std::optional<size_t> CardStack::firstOpenCard() const
	{
		for (size_t i = 0; i < m_cards.size(); ++i)
		{
			if (m_cards[i].state == Card::State::Open)
				return i;
		}
		return {};    // if all cards are flipped, return empty
	}

	const std::vector<Card>& CardStack::cards() const { return m_cards; }

	bool CardStack::append(CardStack&& stack)
	{
		m_cards.insert(m_cards.end(), std::make_move_iterator(stack.m_cards.begin()), std::make_move_iterator(stack.m_cards.end()));
		return true;
	}

	void CardStack::invertOrder() { std::reverse(m_cards.begin(), m_cards.end()); }

	void CardStack::flipAll()
	{
		for (auto& card : m_cards)
			card.flip();
	}

	void CardStack::flipTop()
	{
		if (m_cards.empty())
			return;

		m_cards.back().flip();
	}

	size_t CardStack::size() const { return m_cards.size(); }
}