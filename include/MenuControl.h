#pragma once
#include "GameAction.h"

#include <functional>

namespace panda
{
	class Layout;
	class MenuSelection;
	class Menu;

	class MenuControl
	{
	public:
		MenuControl(const Menu& menu, const Layout& layout, MenuSelection& selection);
		void action(Action action);

	private:
		const Menu& m_menu;
		const Layout& m_layout;
		MenuSelection& m_selection;
	};
}