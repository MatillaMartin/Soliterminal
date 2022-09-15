#include "Render.h"

#include "Card.h"
#include "CardStack.h"
#include "Game.h"
#include "GameControl.h"
#include "GameLayout.h"

#include <assert.h>
#include <iostream>
#include <string>

namespace panda
{
	Render::Render(const GameControl& control, const GameLayout& layout)
		: m_control(control)
		, m_layout(layout)
	{
		update();
	}

	bool Render::isSpread(int x, int y) { return y == 1; }

	std::pair<int, int> Render::position(int x, int y)
	{
		if (x == 0 && y == 4)
			return {0, 5};
		if (x == 0 && y == 5)
			return {0, 6};

		return {x, y};
	}


	void Render::update()
	{
		m_console.begin();

		const StackTable& table = m_layout.table();
		for (int i = 0; i < table.size(); ++i)
		{
			for (int j = 0; j < table[0].size(); ++j)
			{
				auto [iPos, jPos] = position(i, j);
				int x = m_stackSpacing + (m_stackSpacing + m_cardWidth) * iPos;
				int y = m_stackSpacing + (m_stackSpacing + m_cardHeight) * jPos;

				const CardStack& stack = table[i][j]();

				if (stack.cards().empty())
				{
					drawEmpty(x, y);
				}
				else if (isSpread(i, j))
				{
					for (auto it = stack.cards().begin(); it != std::prev(stack.cards().end()); it++)
					{
						drawCardStacked(*it, x, y);
						y += m_cardHeight + m_cardSpacing;
					}

					drawCard(*std::prev(stack.cards().end()), x, y);
				}
				else
				{
					drawCard(*stack.top(), x, y);
				}
			}
		}

		m_console.end();
	}

	std::string cardStr(const Card& card)
	{
		static std::unordered_map<Card::Suit, char> suitMap{
			{Card::Suit::Club, 5},
			{Card::Suit::Diamond, 4},
			{Card::Suit::Heart, 3},
			{Card::Suit::Spade, 6},
		};

		static std::unordered_map<Card::Suit, std::string> suitColorMap{
			{Card::Suit::Club, "\u001b[30m"},
			{Card::Suit::Diamond, "\u001b[31m"},
			{Card::Suit::Heart, "\u001b[31m"},
			{Card::Suit::Spade, "\u001b[30m"},
		};

		auto cardNumberStr = [](int number) -> std::string {
			if (number == 1)
				return "A";
			if (number == 11)
				return "J";
			if (number == 12)
				return "Q";
			if (number == 13)
				return "K";
			return std::to_string(number);
		};

		assert(suitMap.find(card.suit) != suitMap.end());
		if (suitMap.find(card.suit) == suitMap.end())
			return "";
		assert(suitColorMap.find(card.suit) != suitColorMap.end());
		if (suitColorMap.find(card.suit) == suitColorMap.end())
			return "";

		return suitColorMap[card.suit] + cardNumberStr(card.number) + suitMap[card.suit] + "\u001b[0m";
	}

	void Render::drawCard(const Card& card, int x, int y) const
	{
		m_console.drawRect(x, y, m_cardWidth, m_cardHeight);
		m_console.draw(cardStr(card), x + cardCenterX(), y + cardCenterY());
	}

	void Render::drawCardStacked(const Card& card, int x, int y) const
	{
		m_console.drawRectBottomShaded(x, y, m_cardWidth, m_cardHeight);
		m_console.draw(cardStr(card), x + cardCenterX(), y + cardCenterY());
	}


	void Render::drawEmpty(int x, int y) const
	{
		// draw custom emtpy card
		m_console.drawRect(x, y, m_cardWidth, m_cardHeight);
		m_console.draw("[]", x + cardCenterX(), y + cardCenterY());
	}
}