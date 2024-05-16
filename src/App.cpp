#include "App.h"

#include "Card.h"
#include "CardStack.h"
#include "Game.h"
#include "GameControl.h"
#include "GameFileIO.h"
#include "GameRender.h"
#include "Layout.h"
#include "Menu.h"
#include "MenuControl.h"
#include "MenuRender.h"
#include "MenuSelection.h"

namespace panda
{
	App::App() {}

	void App::setState(State state) { m_state = state; }

	App::State App::state() const { return m_state; }
}