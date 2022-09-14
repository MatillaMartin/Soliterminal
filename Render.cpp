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

		int cardWidth = 4;       // spaces per card width, for card like 10
		int cardHeight = 3;      // spaces per card height
		int stackSpacing = 4;    // space between stacks
		int cardSpacing = 0;     // space between cards

		/// Draw top end stacks
		{
			int x = stackSpacing;
			int y = stackSpacing;
			for (const auto& stack : m_game.stacks().endStack)
			{
				if (std::optional<Card> card = stack.top())
					drawCard(*card, x, y);
				else
					drawEmpty(x, y);

				x += stackSpacing + cardWidth;
			}
		}

		/// Draw closed stack
		{
			int x = stackSpacing + (stackSpacing + cardWidth) * 5;
			int y = stackSpacing;
			if (std::optional<Card> card = m_game.stacks().closedStack.top())
				drawCard(*card, x, y);
			else
				drawEmpty(x, y);
		}

		/// Draw open stack
		{
			int x = stackSpacing + (stackSpacing + cardWidth) * 6;
			int y = stackSpacing;
			if (std::optional<Card> card = m_game.stacks().openStack.top())
				drawCard(*card, x, y);
			else
				drawEmpty(x, y);
		}

		/// Draw central stacks
		{
			int x = stackSpacing;
			int startY = stackSpacing + cardHeight + stackSpacing;
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
						y += cardHeight + cardSpacing;
					}

					drawCard(*std::prev(stack.cards().end()), x, y);
				}

				y = startY;
				x += stackSpacing + cardWidth;
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

	void Render::drawCard(const Card& card, int row, int column) const
	{
		// take string from map
		m_console.draw(cardStr(card), row, column);
	}

	void Render::drawCardStacked(const Card& card, int row, int column) const
	{
		// take string from map
		m_console.drawStacked(cardStr(card), row, column);
	}


	void Render::drawEmpty(int row, int column) const
	{
		// draw custom emtpy card
		m_console.draw("[]", row, column);
	}
}