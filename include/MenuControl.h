#pragma once
#include "Action.h"
#include "Layout.h"

#include <functional>

namespace panda
{
	class MenuSelection;
	class Menu;

	class MenuControl : public ActionListener
	{
	public:
		MenuControl(const Menu& menu, MenuSelection& selection);
		void action(const Action& action);

	private:
		const Menu& m_menu;
		Layout m_layout;
		MenuSelection& m_selection;
	};
}