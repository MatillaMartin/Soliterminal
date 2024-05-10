#pragma once
#include "Game.h"
#include "Menu.h"

namespace panda
{
	class App
	{
	public:
		App();

		enum class State
		{
			Game,
			Pause,
			Exit
		};

		void setState(State state);
		State state() const;

	private:
		State m_state = State::Game;
	};
}