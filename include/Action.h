#pragma once

namespace panda
{
	enum class Action
	{
		Up,
		Down,
		Left,
		Right,
		Use,
		None,
		Exit
	};

	class ActionListener
	{
	public:
		virtual void action(const Action& action) = 0;
	};
}