#pragma once

#include "Action.h"
#include "GameSelection.h"

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
		
		GameControl(Game& game, Layout& layout);
		void reset();

		void action(const Action& action);

		size_t stackIndex() const;
		size_t cardIndex() const;

		const GameSelection& selection() const;

	private:
		const CardStack& stack();
		void changeStack(size_t stack);
		// Returns false if the card could not move
		bool changeCard(size_t cardIndex);
		bool isCentralStack();

		Game& m_game;
		const Layout& m_layout;
		GameSelection m_sel;
	};
}