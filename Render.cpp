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

	std::optional<vec2i> Render::layoutToConsole(int index)
	{
		auto layout = m_layout.indexToLayout(index);
		if (!layout)
			return {};
		auto [layoutX, layoutY] = *layout;
		int outX = m_stackSpacing + (m_stackSpacing + m_cardWidth) * layoutX;
		int outY = m_stackSpacing + (m_stackSpacing + m_cardHeight) * layoutY;
		return {vec2i{outX, outY}};
	}

	std::optional<vec2i> Render::position(int stackIndex, int cardIndex)
	{
		auto layout = layoutToConsole(stackIndex);
		if (!layout)
			return {};
		auto [x, y] = *layout;

		if (m_game.isCentralStack(stackIndex))
		{
			y += cardIndex * (m_cardHeight + m_cardSpacing);
		}

		return vec2i{x, y};
	}

	void Render::renderStacks()
	{
		const std::vector<CardStack>& stacks = m_game.stacks();

		// render game layout, with mapping to console positions
		for (int index = 0; index < stacks.size(); ++index)
		{
			const CardStack& stack = stacks[index];

			if (stack.cards().empty())
			{
				auto pos = position(index, 0);
				if (!pos)
					continue;

				if (m_game.isClosedStack(index))
				{
					drawEmptyClosedStack(*pos);
				}
				else
				{
					if (m_game.isCentralStack(index))
						drawEmpty('K', *pos);
					else if (m_game.isEndStack(index))
						drawEmpty('A', *pos);
					else
						drawEmpty(*pos);
				}
			}
			else if (m_game.isCentralStack(index))
			{
				int cardIndex = 0;
				for (auto it = stack.cards().begin(); it != std::prev(stack.cards().end()); it++)
				{
					auto pos = position(index, cardIndex);
					if (!pos)
						continue;
					drawCardSpread(*it, *pos);
					cardIndex++;
				}

				auto pos = position(index, cardIndex);
				if (!pos)
					continue;
				drawCard(*std::prev(stack.cards().end()), *pos);
			}
			else
			{
				auto pos = position(index, 0);
				if (!pos)
					continue;
				drawCard(*stack.top(), *pos);
			}
		}
	}

	void Render::renderControlSelect()
	{
		auto pos = position(m_control.stackIndex(), m_control.cardIndex());
		if (!pos)
			return;
		drawControlSelect(*pos);
	}

	void Render::renderControlMark()
	{
		auto pos = position(m_control.markedStackIndex(), m_control.markedCardIndex());
		if (!pos)
			return;
		drawControlMark(*pos);
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

	void Render::drawCard(const Card& card, vec2i pos)
	{
		if (card.state == Card::State::Closed)
		{
			m_console.setDrawColor(m_closedColorFg, m_closedColorBg);
			m_console.drawRectWithCrosses(pos.first, pos.second, m_cardWidth, m_cardHeight);
		}
		else
		{
			m_console.setDrawColor(0x0, m_openColorFg);
			m_console.drawRect(pos.first, pos.second, m_cardWidth, m_cardHeight);
			m_console.draw(cardStr(card), pos.first + cardCenterX(), pos.second + cardCenterY());
		}
	}

	void Render::drawCardSpread(const Card& card, vec2i pos)
	{
		drawCard(card, pos);

		if (card.state == Card::State::Closed)
		{
			m_console.setDrawColor(0x0, m_closedColorBg);
			drawShade(pos);
		}
		else
		{
			m_console.setDrawColor(0x0, m_openColorFg);
			drawShade(pos);
		}
	}

	void Render::drawEmpty(char text, vec2i pos)
	{
		// draw custom emtpy card
		m_console.setDrawColor(m_emptyColorFg);
		drawEmpty(pos);
		m_console.draw(text, pos.first + cardCenterX(), pos.second + cardCenterY());
	}

	void Render::drawEmpty(vec2i pos)
	{
		// draw custom emtpy card
		m_console.setDrawColor(m_emptyColorFg);
		m_console.drawRectOutline(pos.first, pos.second, m_cardWidth, m_cardHeight);
	}


	void Render::drawEmptyClosedStack(vec2i pos)
	{
		// draw custom emtpy card with arrow inside
		m_console.setDrawColor(m_emptyColorFg);
		m_console.drawRectOutline(pos.first, pos.second, m_cardWidth, m_cardHeight);
		m_console.draw(char(174), pos.first + cardCenterX(), pos.second + cardCenterY());
	}

	void Render::drawControlSelect(vec2i pos)
	{
		// draw control with select color
		m_console.setDrawColor(m_selectColor);
		drawControl(pos);
	}

	void Render::drawControlMark(vec2i pos)
	{
		// draw control with mark color
		m_console.setDrawColor(m_markColor);
		drawControl(pos);
	}

	void Render::drawControl(vec2i pos)
	{
		// draw control as arrows pointing to card
		m_console.draw(char(16), pos.first - 1, pos.second + cardCenterY());
		m_console.draw(char(17), pos.first + m_cardWidth, pos.second + cardCenterY());
	}

	void Render::drawShade(vec2i pos)
	{
		for (int i = 0; i < m_cardWidth; ++i)
			m_console.draw("_", pos.first + i, pos.second + m_cardHeight - 1);
	}
}