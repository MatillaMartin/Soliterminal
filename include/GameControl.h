#pragma once

#include "Action.h"

#include <optional>
#include <utility>

namespace panda
{
	class CardStack;
	class Layout;
	class Game;

	class GameControl : public ActionListener
	{
	public:
		enum class State
		{
			Select,
			Move
		};

		GameControl(Game& game, Layout& layout);
		void reset();

		void action(const Action& action);

		size_t stackIndex() const;
		size_t cardIndex() const;

		size_t markedStackIndex() const;
		size_t markedCardIndex() const;

		State state() const;

	private:
		const CardStack& stack();
		void changeStack(size_t stack);
		// Returns false if the card could not move
		bool changeCard(size_t cardIndex);
		bool isCentralStack();

		Game& m_game;
		const Layout& m_layout;
		size_t m_cardIndex = 0;
		size_t m_stackIndex = 0;

		State m_state = State::Select;
		size_t m_markedCardIndex = 0;
		size_t m_markedStackIndex = 0;
	};
}