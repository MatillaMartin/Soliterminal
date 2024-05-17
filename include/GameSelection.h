#pragma once

namespace panda
{
	class GameSelection
	{
	public:
		enum class State
		{
			Select,
			Move
		};

		void reset();

		State state = State::Select;
		size_t cardIndex = 0;
		size_t stackIndex = 0;
		size_t markedCardIndex = 0;
		size_t markedStackIndex = 0;
	};
}