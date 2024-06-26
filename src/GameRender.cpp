#include "GameRender.h"

#include "Card.h"
#include "CardStack.h"
#include "Game.h"
#include "GameControl.h"
#include "Layout.h"

#include <assert.h>
#include <iostream>
#include <string>

namespace panda
{
	GameRender::GameRender(const Game& game, const GameSelection& selection, const Layout& layout, Console& console)
		: m_game(game)
		, m_selection(selection)
		, m_layout(layout)
		, m_console(console)
	{
		m_console.setClearColor(m_clearColor);
		update();
	}

	std::optional<vec2i> GameRender::indexToConsole(size_t index)
	{
		auto layout = m_layout.indexToLayout(index);
		if (!layout)
			return {};
		auto [layoutX, layoutY] = *layout;
		int outX = m_stackSpacing + (m_stackSpacing + m_cardWidth) * layoutX;
		int outY = m_stackSpacing + (m_stackSpacing + m_cardHeight) * layoutY;
		return {vec2i{outX, outY}};
	}

	std::optional<vec2i> GameRender::position(size_t stackIndex, size_t cardIndex)
	{
		auto layout = indexToConsole(stackIndex);
		if (!layout)
			return {};
		auto [x, y] = *layout;

		const std::vector<CardStack>& stacks = m_game.stacks();
		const CardStack& stack = stacks[stackIndex];

		if (m_game.isCentralStack(stackIndex))
		{
			auto openCardIndex = stack.firstOpenCard();
			if (!openCardIndex)    // all closed
			{
				y += static_cast<int>(cardIndex) * m_cardSpreadHeight;
			}
			else    // some closed, some open
			{
				if (cardIndex > *openCardIndex)
				{
					// add up the closed cards
					y += static_cast<int>(*openCardIndex) * m_cardSpreadHeight;
					// add up the open cards
					y += (static_cast<int>(cardIndex) - static_cast<int>(*openCardIndex)) * m_cardHeight;
				}
				else
				{
					y += static_cast<int>(cardIndex) * m_cardSpreadHeight;
				}
			}
		}

		return vec2i{x, y};
	}

	void GameRender::renderStacks()
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

	void GameRender::renderControlSelect()
	{
		auto pos = position(m_selection.stackIndex, m_selection.cardIndex);
		if (!pos)
			return;
		drawControlSelect(*pos);
	}

	void GameRender::renderControlMark()
	{
		auto pos = position(m_selection.markedStackIndex, m_selection.markedCardIndex);
		if (!pos)
			return;
		drawControlMark(*pos);
	}

	void GameRender::update()
	{
		m_console.begin();
		renderStacks();
		// render game control
		{
			// Draw control select always
			renderControlSelect();

			if (m_selection.state == GameSelection::State::Move)
			{
				renderControlMark();
			}
		}

		m_console.end();
	}

	std::optional<int> cardColor(const Card& card)
	{
		static std::unordered_map<Card::Suit, int> suitColorMap{
			{Card::Suit::Heart, 0x4},
			{Card::Suit::Diamond, 0x4},
			{Card::Suit::Club, 0x0},
			{Card::Suit::Spade, 0x0},
		};

		assert(suitColorMap.find(card.suit) != suitColorMap.end());
		auto it = suitColorMap.find(card.suit);
		if (it == suitColorMap.end())
			return {};
		return it->second;
	}

	std::string cardStr(const Card& card)
	{
		static std::unordered_map<Card::Suit, char> suitMap{
			{Card::Suit::Heart, 3},
			{Card::Suit::Diamond, 4},
			{Card::Suit::Club, 5},
			{Card::Suit::Spade, 6},
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

		return cardNumberStr(card.number) + suitMap[card.suit];
	}

	void GameRender::drawCard(const Card& card, vec2i pos)
	{
		if (card.state == Card::State::Closed)
		{
			m_console.setDrawColor(m_closedColorFg, m_closedColorBg);
			m_console.drawRect(pos.first, pos.second, m_cardWidth, m_cardHeight);
			m_console.draw(char(206), pos.first + cardCenterX(), pos.second + cardCenterY());
			m_console.draw(char(206), pos.first + cardCenterX() + 1, pos.second + cardCenterY());
		}
		else
		{
			auto fgColor = cardColor(card);
			if (!fgColor)
				return;
			m_console.setDrawColor(*fgColor, m_openColorFg);
			m_console.drawRect(pos.first, pos.second, m_cardWidth, m_cardHeight);
			m_console.draw(cardStr(card), pos.first + cardCenterX(), pos.second + cardCenterY());
		}
	}

	void GameRender::drawCardSpread(const Card& card, vec2i pos)
	{
		drawCard(card, pos);

		if (card.state == Card::State::Closed)
		{
			m_console.setDrawColor(0x0, m_closedColorBg);
			drawShade(pos.first, pos.second);
		}
		else
		{
			m_console.setDrawColor(0x0, m_openColorFg);
			drawShade(pos.first, pos.second + m_cardHeight - 1);
		}
	}

	void GameRender::drawEmpty(char text, vec2i pos)
	{
		// draw custom emtpy card
		m_console.setDrawColor(m_emptyColorFg);
		drawEmpty(pos);
		m_console.draw(text, pos.first + cardCenterX(), pos.second + cardCenterY());
	}

	void GameRender::drawEmpty(vec2i pos)
	{
		// draw custom emtpy card
		m_console.setDrawColor(m_emptyColorFg);
		m_console.drawRectOutline(pos.first, pos.second, m_cardWidth, m_cardHeight);
	}


	void GameRender::drawEmptyClosedStack(vec2i pos)
	{
		// draw custom emtpy card with arrow inside
		m_console.setDrawColor(m_emptyColorFg);
		m_console.drawRectOutline(pos.first, pos.second, m_cardWidth, m_cardHeight);
		m_console.draw(char(174), pos.first + cardCenterX(), pos.second + cardCenterY());
	}

	void GameRender::drawControlSelect(vec2i pos)
	{
		// draw control with select color
		m_console.setDrawColor(m_selectColor);
		drawControl(pos);
	}

	void GameRender::drawControlMark(vec2i pos)
	{
		// draw control with mark color
		m_console.setDrawColor(m_markColor);
		drawControl(pos);
	}

	void GameRender::drawControl(vec2i pos)
	{
		// draw control as arrows pointing to card
		m_console.draw(char(16), pos.first - 1, pos.second + cardCenterY());
		m_console.draw(char(17), pos.first + m_cardWidth, pos.second + cardCenterY());
	}

	void GameRender::drawShade(int x, int y)
	{
		for (int i = 0; i < m_cardWidth; ++i)
			m_console.draw("_", x + i, y);
	}
}