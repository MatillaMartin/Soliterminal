#pragma once

#include "GameAction.h"

#include <optional>
#include <utility>

namespace panda
{
	class CardStack;
	class Layout;
	class Game;

	class GameControl
	{
	public:
		enum class State
		{
			Select,
			Move
		};

		GameControl(Game& game, Layout& layout);

		void action(GameAction action);

		int stackIndex() const;
		int cardIndex() const;

		int markedStackIndex() const;
		int markedCardIndex() const;

		State state() const;

	private:
		const CardStack& stack();
		void changeStack(int stack);
		// Returns false if the card could not move
		bool changeCard(int cardIndex);
		bool isCentralStack();

		Game& m_game;
		const Layout& m_layout;
		int m_cardIndex = 0;
		int m_stackIndex = 0;

		State m_state = State::Select;
		int m_markedCardIndex = 0;
		int m_markedStackIndex = 0;
	};
}