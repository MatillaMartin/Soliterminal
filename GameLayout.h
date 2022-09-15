#pragma once

#include <array>
#include <functional>

namespace panda
{
	class Game;
	class CardStack;

	typedef std::array<std::function<const CardStack&()>, 12> StackList;

	class GameLayout
	{
	public:
		GameLayout(Game& game);

		// Returns the list of stacks. Use the layout given by indexToLayout
		const StackList& stacks() const;

		// Mapping between the layout and stacks index
		int layoutToIndex(int x, int y) const;

		// Mapping between stacks index and layout
		std::pair<int, int> indexToLayout(int index) const;

		// Returns the index of the element up of index
		int up(int index) const;

		// Returns the index of the element down of index
		int down(int index) const;

		// Returns the index of the element left of index
		int left(int index) const;

		// Returns the index of the element right of index
		int right(int index) const;

		// Returns true if the index matches an end stack
		bool isEndStack(int index) const;

		// Returns true if the index matches a central stack
		bool isCentralStack(int index) const;

		// Returns true if the index matches the OpenStack
		bool isOpenStack(int index) const;

		// Returns true if the index matches the ClosedStack
		bool isClosedStack(int index) const;

		// Returns the index for the OpenStack
		int openStack() const;

		// Returns the index for the ClosedStack
		int closedStack() const;

	private:
		// Mapping between the grid position and stacks list
		int gridToIndex(int x, int y) const;

		// Mapping between grid and stacks list
		std::pair<int, int> indexToGrid(int index) const;

		// Mapping between grid and physical layout
		std::pair<int, int> gridToLayout(int x, int y) const;

		// Mapping between grid and physical layout
		std::pair<int, int> layoutToGrid(int x, int y) const;

		// Offsets the index by the given amount
		int offset(int index, int dx, int dy) const;

		Game& m_game;
		int m_gridWidth = 6;

	};
}