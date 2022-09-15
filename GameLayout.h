#pragma once

#include <array>
#include <functional>

namespace panda
{
	class Game;
	class CardStack;

	typedef std::array<std::array<std::function<const CardStack&()>, 2>, 6> StackTable;

	class GameLayout
	{
	public:
		GameLayout(Game& game);

		const StackTable& table() const;

		// Mapping between table memory and position of the element
		std::pair<int, int> gridToTable(int x, int y) const;
		std::pair<int, int> tableToGrid(int x, int y) const;

	private:
		Game& m_game;
	};
}