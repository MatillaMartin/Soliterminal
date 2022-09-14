#include "Render.h"

#include "Card.h"
#include "CardStack.h"
#include "Game.h"

#include <assert.h>
#include <string>
#include <unordered_map>
#include <iostream>

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

		int cardWidth = 3;    // 2 spaces per card, for card like 10
		int spacing = 2;      // 1 space between stacks

		/// Draw top end stacks
		{
			int x = spacing;
			int y = spacing;
			for (const auto& stack : m_game.stacks().endStack)
			{
				if (std::optional<Card> card = stack.top())
					drawCard(*card, x, y);
				else
					drawEmpty(x, y);

				x += spacing + cardWidth;
			}
		}

		/// Draw closed stack
		{
			int x = spacing + (spacing + cardWidth) * 5;
			int y = spacing;
			if (std::optional<Card> card = m_game.stacks().closedStack.top())
				drawCard(*card, x, y);
			else
				drawEmpty(x, y);
		}

		/// Draw open stack
		{
			int x = spacing + (spacing + cardWidth) * 6;
			int y = spacing;
			if (std::optional<Card> card = m_game.stacks().openStack.top())
				drawCard(*card, x, y);
			else
				drawEmpty(x, y);
		}

		/// Draw central stacks
		{
			int x = spacing;
			int startY = spacing + 1 + spacing;
			int y = startY;

			for (const auto& stack : m_game.stacks().centralStack)
			{
				for (const auto& card : stack.cards())
				{
					drawCard(card, x, y);
					y += 1;
				}

				y = startY;
				x += spacing + cardWidth;
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
			{Card::Suit::Club, "\u001b[33m"},
			{Card::Suit::Diamond, "\u001b[31m"},
			{Card::Suit::Heart, "\u001b[31m"},
			{Card::Suit::Spade, "\u001b[33m"},
		};

		auto cardNumberStr = [](int number) -> std::string {
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

	void Render::drawEmpty(int row, int column) const
	{
		// draw custom emtpy card
		m_console.draw("[]", row, column);
	}
}