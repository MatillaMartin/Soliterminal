#include "Render.h"

namespace panda
{
	Render::Render(const Game& game)
		: m_game(game)
	{
		update();
	}

	void Render::update()
	{
		// clear console
		// render current game state
	}
}