#pragma once

namespace panda
{
	class Game;

	class Render
	{
	public:
		Render(const Game& game);
		void update();

	private:
		const Game& m_game;
	};
}