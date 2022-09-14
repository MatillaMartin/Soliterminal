#include "Render.h"

#include "Card.h"
#include "CardStack.h"
#include "Game.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <unordered_map>

namespace panda
{
	Render::Render(const Game& game)
		: m_game(game)
	{
		update();
	}

	void Render::update()
	{
		m_console.begin();

		/// Draw top end stacks
		{
			int x = m_stackSpacing;
			int y = m_stackSpacing;
			for (const auto& stack : m_game.stacks().endStack)
			{
				if (std::optional<Card> card = stack.top())
					drawCard(*card, x, y);
				else
					drawEmpty(x, y);

				x += m_stackSpacing + m_cardWidth;
			}
		}

		/// Draw closed stack
		{
			int x = m_stackSpacing + (m_stackSpacing + m_cardWidth) * 5;
			int y = m_stackSpacing;
			if (std::optional<Card> card = m_game.stacks().closedStack.top())
				drawCard(*card, x, y);
			else
				drawEmpty(x, y);
		}

		/// Draw open stack
		{
			int x = m_stackSpacing + (m_stackSpacing + m_cardWidth) * 6;
			int y = m_stackSpacing;
			if (std::optional<Card> card = m_game.stacks().openStack.top())
				drawCard(*card, x, y);
			else
				drawEmpty(x, y);
		}

		/// Draw central stacks
		{
			int x = m_stackSpacing;
			int startY = m_stackSpacing + m_cardHeight + m_stackSpacing;
			int y = startY;

			for (const auto& stack : m_game.stacks().centralStack)
			{
				if (stack.cards().empty())
				{
					drawEmpty(x, y);
				}
				else
				{
					for (auto it = stack.cards().begin(); it != std::prev(stack.cards().end()); it++)
					{
						drawCardStacked(*it, x, y);
						y += m_cardHeight + m_cardSpacing;
					}

					drawCard(*std::prev(stack.cards().end()), x, y);
				}

				y = startY;
				x += m_stackSpacing + m_cardWidth;
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