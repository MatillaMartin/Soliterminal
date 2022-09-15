#include "GameLayout.h"

#include "Game.h"

namespace panda
{
	GameLayout::GameLayout(Game& game)
		: m_game(game)
	{
	}

	const StackTable& GameLayout::table() const
	{
		// define layout of stacks, action moves between layout
		static StackTable stackTable{
			std::array<std::function<const CardStack&()>, 2>{[this]() -> const CardStack& { return m_game.stacks().endStack[0]; },
															 [this]() -> const CardStack& { return m_game.stacks().centralStack[0]; }},

			std::array<std::function<const CardStack&()>, 2>{[this]() -> const CardStack& { return m_game.stacks().endStack[1]; },
															 [this]() -> const CardStack& { return m_game.stacks().centralStack[1]; }},

			std::array<std::function<const CardStack&()>, 2>{[this]() -> const CardStack& { return m_game.stacks().endStack[2]; },
															 [this]() -> const CardStack& { return m_game.stacks().centralStack[2]; }},

			std::array<std::function<const CardStack&()>, 2>{[this]() -> const CardStack& { return m_game.stacks().endStack[3]; },
															 [this]() -> const CardStack& { return m_game.stacks().centralStack[3]; }},

			std::array<std::function<const CardStack&()>, 2>{[this]() -> const CardStack& { return m_game.stacks().openStack; },
															 [this]() -> const CardStack& { return m_game.stacks().centralStack[4]; }},

			std::array<std::function<const CardStack&()>, 2>{[this]() -> const CardStack& { return m_game.stacks().closedStack; },
															 [this]() -> const CardStack& { return m_game.stacks().centralStack[5]; }},
		};

		return stackTable;
	}
}