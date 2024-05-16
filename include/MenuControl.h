#pragma once
#include "Action.h"
#include "Layout.h"
#include "MenuSelection.h"

#include <functional>

namespace panda
{
	class Menu;

	class MenuControl : public ActionListener
	{
	public:
		MenuControl(const Menu& menu);
		void action(const Action& action);
		const MenuSelection& selection() const;
	private:
		const Menu& m_menu;
		Layout m_layout;
		MenuSelection m_selection;
	};
}