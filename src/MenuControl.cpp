#include "MenuControl.h"

#include "Layout.h"
#include "Menu.h"
#include "MenuSelection.h"

namespace panda
{
	MenuControl::MenuControl(const Menu& menu, const Layout& layout, MenuSelection& selection)
		: m_menu(menu)
		, m_layout(layout)
		, m_selection(selection)
	{
	}

	void MenuControl::action(const Action& action)
	{
		switch (action)
		{
		case Action::Up:
			m_selection.setIndex(m_layout.up(m_selection.index()));
			break;
		case Action::Down:
			m_selection.setIndex(m_layout.down(m_selection.index()));
			break;
		case Action::Use:
		{
			auto opts = m_menu.options();
			if (m_selection.index() < opts.size())
			{
				if (auto func = opts.at(m_selection.index()).second)
					func();
			}
		}
		break;
		default:
			break;
		}
	}
}
