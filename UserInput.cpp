#include "UserInput.h"

namespace panda
{
	namespace
	{
		const int KEY_UP = 72;
		const int KEY_DOWN = 80;
		const int KEY_LEFT = 75;
		const int KEY_RIGHT = 77;
	}

	GameAction UserInput::waitForInput()
	{
		while (true)
		{
			int c = _getch();

			if (c == KEY_UP)
				return GameAction::Up;
			if (c == KEY_DOWN)
				return GameAction::Down;
			if (c == KEY_LEFT)
				return GameAction::Left;
			if (c == KEY_RIGHT)
				return GameAction::Right;
			if (c == ' ')
				return GameAction::Use;
		}

		return GameAction::None;
	}
}