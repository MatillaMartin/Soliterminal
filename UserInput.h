#pragma once
#include <conio.h>
#include "GameAction.h"

namespace panda
{
	class UserInput
	{
	public:
		static GameAction waitForInput();
	};
}