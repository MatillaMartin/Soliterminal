#include "UserInput.h"

namespace panda
{
	namespace
	{
		const int KEY_UP = 72;
		const int KEY_DOWN = 80;
		const int KEY_LEFT = 75;
		const int KEY_RIGHT = 77;
		const int KEY_ESC = 27;
	}

	Action UserInput::waitForInput()
	{
		while (true)
		{
			int c = _getch();

			if (c == KEY_UP)
				return Action::Up;
			if (c == KEY_DOWN)
				return Action::Down;
			if (c == KEY_LEFT)
				return Action::Left;
			if (c == KEY_RIGHT)
				return Action::Right;
			if (c == ' ')
				return Action::Use;
			if (c == 'r')
				return Action::Reset;
			if (c == KEY_ESC)
				return Action::Exit;
		}

		return Action::None;
	}
}