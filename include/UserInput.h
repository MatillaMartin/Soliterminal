#pragma once
#include "Action.h"

#include <conio.h>

namespace panda
{
	class UserInput
	{
	public:
		static Action waitForInput();
	};
}