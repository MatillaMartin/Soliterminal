#include "CardStack.h"

#include "Card.h"

namespace panda
{
	CardStack::CardStack(const std::vector<Card>& cards)
		: m_cards(cards.begin(), cards.end())
	{
	}

	std::optional<CardStack> CardStack::take(int index)
	{
		if (index >= m_cards.size())
			return {};

		auto first = m_cards.begin();
		std::advance(first, index);
		return std::vector<Card>(first, m_cards.end());
	}

	std::optional<CardStack> CardStack::takeTop()
	{
		if (m_cards.empty())
			return {};

		return take(m_cards.size() - 1);
	}

	bool CardStack::append(CardStack&& stack) 
	{
		m_cards.splice(m_cards.end(), stack.m_cards); 
		return true;
	}

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

	int CardStack::size() { return m_cards.size(); }
}