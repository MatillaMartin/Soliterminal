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
		m_console.setClearColor(m_clearColor);
		update();
	}

	std::optional<std::pair<int, int>> Render::layoutToConsole(int index)
	{
		auto layout = m_layout.indexToLayout(index);
		if (!layout)
			return {};
		auto [layoutX, layoutY] = *layout;
		int outX = m_stackSpacing + (m_stackSpacing + m_cardWidth) * layoutX;
		int outY = m_stackSpacing + (m_stackSpacing + m_cardHeight) * layoutY;
		return {std::pair<int, int>{outX, outY}};
	}

	void Render::renderStacks()
	{
		const std::vector<CardStack>& stacks = m_game.stacks();

		// render game layout, with mapping to console positions
		for (int index = 0; index < stacks.size(); ++index)
		{
			auto layout = layoutToConsole(index);
			if (!layout)
				continue;
			auto [x, y] = *layout;

			const CardStack& stack = stacks[index];

			if (stack.cards().empty())
			{
				if (m_game.isClosedStack(index))
				{
					drawEmptyClosedStack(x, y);
				}
				else
				{
					if (m_game.isCentralStack(index))
						drawEmpty('K', x, y);
					else if (m_game.isEndStack(index))
						drawEmpty('A', x, y);
					else
						drawEmpty(x, y);
				}
			}
			else if (m_game.isCentralStack(index))
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
	}

	void Render::renderControlSelect()
	{
		int index = m_control.stackIndex();
		auto layout = layoutToConsole(index);
		if (!layout)
			return;
		auto [x, y] = *layout;
		if (m_game.isCentralStack(index))
		{
			y += (m_cardHeight + m_cardSpacing) * m_control.cardIndex();
		}
		drawControlSelect(x, y);
	}

	void Render::renderControlMark()
	{
		// Draw control select always
		int index = m_control.markedStackIndex();
		auto layout = layoutToConsole(index);
		if (!layout)
			return;
		auto [x, y] = *layout;
		if (m_game.isCentralStack(index))
		{
			y += (m_cardHeight + m_cardSpacing) * m_control.markedCardIndex();
		}
		drawControlMark(x, y);
	}

	void Render::update()
	{
		m_console.begin();
		renderStacks();
		// render game control
		{
			// Draw control select always
			renderControlSelect();

			if (m_control.state() == GameControl::State::Move)
			{
				renderControlMark();
			}
		}

		//std::cout << std::endl << std::endl << std::endl << std::endl;
		//for (int i = 0; i < 256; ++i)
		//{
		//	std::cout << i << ": " << char(i) << "\t";
		//}

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

	void Render::drawCard(const Card& card, int x, int y)
	{
		if (card.state == Card::State::Closed)
		{
			m_console.setDrawColor(m_closedColorFg, m_closedColorBg);
			m_console.drawRectWithCrosses(x, y, m_cardWidth, m_cardHeight);
		}
		else
		{
			m_console.setDrawColor(0x0, m_openColorFg);
			m_console.drawRect(x, y, m_cardWidth, m_cardHeight);
			m_console.draw(cardStr(card), x + cardCenterX(), y + cardCenterY());
		}
	}

	void Render::drawCardSpread(const Card& card, int x, int y)
	{
		drawCard(card, x, y);

		if (card.state == Card::State::Closed)
		{
			m_console.setDrawColor(0x0, m_closedColorBg);
			drawShade(x, y);
		}
		else
		{
			m_console.setDrawColor(0x0, m_openColorFg);
			drawShade(x, y);
		}
	}

	void Render::drawEmpty(char text, int x, int y)
	{
		// draw custom emtpy card
		m_console.setDrawColor(m_emptyColorFg);
		drawEmpty(x, y);
		m_console.draw(text, x + cardCenterX(), y + cardCenterY());
	}

	void Render::drawEmpty(int x, int y)
	{
		// draw custom emtpy card
		m_console.setDrawColor(m_emptyColorFg);
		m_console.drawRectOutline(x, y, m_cardWidth, m_cardHeight);
	}


	void Render::drawEmptyClosedStack(int x, int y)
	{
		// draw custom emtpy card with arrow inside
		m_console.setDrawColor(m_emptyColorFg);
		m_console.drawRectOutline(x, y, m_cardWidth, m_cardHeight);
		m_console.draw(char(174), x + cardCenterX(), y + cardCenterY());
	}

	void Render::drawControlSelect(int x, int y)
	{
		// draw control with select color
		m_console.setDrawColor(m_selectColor);
		drawControl(x, y);
	}

	void Render::drawControlMark(int x, int y)
	{
		// draw control with mark color
		m_console.setDrawColor(m_markColor);
		drawControl(x, y);
	}

	void Render::drawControl(int x, int y)
	{
		m_console.draw(char(16), x - 1, y + cardCenterY());
		m_console.draw(char(17), x + m_cardWidth, y + cardCenterY());
	}

	void Render::drawShade(int x, int y)
	{
		for (int i = 0; i < m_cardWidth; ++i)
			m_console.draw("_", x + i, y + m_cardHeight - 1);
	}
}