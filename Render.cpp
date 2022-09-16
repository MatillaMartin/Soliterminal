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
	Render::Render(const Game& game, const GameControl& control, const GameLayout& layout)
		: m_game(game)
		, m_control(control)
		, m_layout(layout)
	{
		update();
	}

	std::pair<int, int> Render::layoutToConsole(int index)
	{
		auto [layoutX, layoutY] = m_layout.indexToLayout(index);
		int outX = m_stackSpacing + (m_stackSpacing + m_cardWidth) * layoutX;
		int outY = m_stackSpacing + (m_stackSpacing + m_cardHeight) * layoutY;
		return {outX, outY};
	}

	void Render::update()
	{
		auto isSpread = [this](int index) { return m_game.isCentralStack(index); };

		m_console.begin();

		const std::vector<CardStack>& stacks = m_game.stacks();
		// render game layout, with mapping to console positions
		for (int index = 0; index < stacks.size(); ++index)
		{
			auto [x, y] = layoutToConsole(index);

			const CardStack& stack = stacks[index];

			if (stack.cards().empty())
			{
				drawEmpty(x, y);
			}
			else if (isSpread(index))
			{
				for (auto it = stack.cards().begin(); it != std::prev(stack.cards().end()); it++)
				{
					drawCardSpread(*it, x, y);
					y += m_cardHeight + m_cardSpacing;
				}

				drawCard(*std::prev(stack.cards().end()), x, y);
			}
			else
			{
				drawCard(*stack.top(), x, y);
			}
		}

		// render game control
		{
			// Draw control select always
			{
				int index = m_control.stackIndex();
				auto [x, y] = layoutToConsole(index);
				if (isSpread(index))
				{
					y += (m_cardHeight + m_cardSpacing) * m_control.cardIndex();
				}
				drawControlSelect(x, y);
			}

			if (m_control.state() == GameControl::State::Move)
			{
				// Draw control select always
				int index = m_control.markedStackIndex();
				auto [x, y] = layoutToConsole(index);
				if (isSpread(index))
				{
					y += (m_cardHeight + m_cardSpacing) * m_control.markedCardIndex();
				}
				drawControlMark(x, y);
			}
		}


		m_console.end();
	}

	std::string cardStr(const Card& card)
	{
		static std::unordered_map<Card::Suit, char> suitMap{
			{Card::Suit::Heart, 3},
			{Card::Suit::Diamond, 4},
			{Card::Suit::Club, 5},
			{Card::Suit::Spade, 6},
		};

		static std::unordered_map<Card::Suit, std::string> suitColorMap{
			{Card::Suit::Heart, "\u001b[31m"},
			{Card::Suit::Diamond, "\u001b[31m"},
			{Card::Suit::Club, "\u001b[30m"},
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

	void Render::drawCardSpread(const Card& card, int x, int y) const
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

	void Render::drawControlSelect(int x, int y) const
	{
		// draw custom emtpy card
		m_console.drawRectHighlightBlue(x, y, m_cardWidth, m_cardHeight);
	}

	void Render::drawControlMark(int x, int y) const
	{
		// draw custom emtpy card
		m_console.drawRectHighlightGreen(x, y, m_cardWidth, m_cardHeight);
	}
}