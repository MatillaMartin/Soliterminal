#pragma once
#include "GameAction.h"

#include <conio.h>

namespace panda
{
	class UserInput
	{
	public:
		static Action waitForInput();
	};
}