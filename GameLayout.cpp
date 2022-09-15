#include "GameLayout.h"

#include "Game.h"

#include <algorithm>

namespace panda
{
	GameLayout::GameLayout(Game& game)
		: m_game(game)
	{
	}

	const StackList& GameLayout::stacks() const
	{
		// define layout of stacks as a table
		static StackList stackTable{
			[this]() -> const CardStack& { return m_game.stacks().endStack[0]; },
			[this]() -> const CardStack& { return m_game.stacks().endStack[1]; },
			[this]() -> const CardStack& { return m_game.stacks().endStack[2]; },
			[this]() -> const CardStack& { return m_game.stacks().endStack[3]; },
			[this]() -> const CardStack& { return m_game.stacks().openStack; },
			[this]() -> const CardStack& { return m_game.stacks().closedStack; },

			[this]() -> const CardStack& { return m_game.stacks().centralStack[0]; },
			[this]() -> const CardStack& { return m_game.stacks().centralStack[1]; },
			[this]() -> const CardStack& { return m_game.stacks().centralStack[2]; },
			[this]() -> const CardStack& { return m_game.stacks().centralStack[3]; },
			[this]() -> const CardStack& { return m_game.stacks().centralStack[4]; },
			[this]() -> const CardStack& { return m_game.stacks().centralStack[5]; },
		};

		return stackTable;
	}

	// Mapping between the layout and stacks index
	int GameLayout::layoutToIndex(int x, int y) const
	{
		auto [gridX, gridY] = layoutToGrid(x, y);
		return gridToIndex(gridX, gridY);
	}

	// Mapping between stacks index and layout
	std::pair<int, int> GameLayout::indexToLayout(int index) const 
	{
		auto [gridX, gridY] = indexToGrid(index);
		return gridToLayout(gridX, gridY);
	}

	int GameLayout::gridToIndex(int x, int y) const
	{
		auto subToInd = [this](int x, int y) -> int { return x + y * m_gridWidth; };
		return subToInd(x, y);
	}

	std::pair<int, int> GameLayout::indexToGrid(int index) const
	{
		int x = index % m_gridWidth;
		int y = index / m_gridWidth;
		return {x, y};
	}

	std::pair<int, int> GameLayout::gridToLayout(int x, int y) const
	{
		if (x == 4 && y == 0)
			return {5, 0};
		if (x == 5 && y == 0)
			return {6, 0};

		return {x, y};
	}

	std::pair<int, int> GameLayout::layoutToGrid(int x, int y) const
	{
		if (x == 6 && y == 0)
			return {5, 0};
		if (x == 5 && y == 0)
			return {4, 0};

		return {x, y};
	}

	int GameLayout::up(int index) const { return offset(index, 0, -1); }

	int GameLayout::down(int index) const { return offset(index, 0, 1); }

	int GameLayout::left(int index) const { return offset(index, -1, 0); }

	int GameLayout::right(int index) const { return offset(index, 1, 0); }

	bool GameLayout::isEndStack(int index) const { return index >= 0 && index < 4; }

	bool GameLayout::isCentralStack(int index) const { return index >= 6 && index < 12; }

	bool GameLayout::isOpenStack(int index) const { return index == openStack(); }

	bool GameLayout::isClosedStack(int index) const { return index == closedStack(); }

	int GameLayout::openStack() const { return 4; };

	int GameLayout::closedStack() const { return 5; }

	int GameLayout::offset(int index, int dx, int dy) const
	{
		auto [gridX, gridY] = indexToGrid(index);

		int gridOutX = gridX + dx;
		int gridOutY = gridY + dy;

		// special case of displaced layouts
		if (gridX == 4 && gridY == 0 && gridOutY == 1)
			gridOutX = 5;

		if (gridX == 5 && gridY == 0 && gridOutY == 1)
			gridOutX = 5;

		if (gridX == 5 && gridY == 1 && gridOutY == 0)
			gridOutX = 4;

		gridOutX = std::clamp(gridOutX, 0, m_gridWidth - 1);
		gridOutY = std::clamp(gridOutY, 0, m_gridWidth - 1);

		return gridToIndex(gridOutX, gridOutY);
	};
}
