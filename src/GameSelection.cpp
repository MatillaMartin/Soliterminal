#include "GameSelection.h"

namespace panda
{
	void GameSelection::reset()
	{
		state = State::Select;
		cardIndex = 0;
		stackIndex = 0;
		markedCardIndex = 0;
		markedStackIndex = 0;
	}
}