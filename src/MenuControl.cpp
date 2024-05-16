#include "MenuControl.h"

#include "Layout.h"
#include "Menu.h"
#include "MenuSelection.h"

#include <numeric>
#include <vector>

namespace panda
{
	namespace
	{
		Layout createMenuLayout(size_t numOptions)
		{
			std::vector<size_t> chain;
			chain.resize(numOptions);
			std::iota(chain.begin(), chain.end(), 0);
			chain.push_back(0);    // circle back to start
			Graph graph;
			for (size_t i = 0; i < numOptions; ++i)
			{
				graph.addNode(i, {0, i});
			}
			graph.addVerChain(chain);
			return Layout(std::move(graph));
		}
	}

	MenuControl::MenuControl(const Menu& menu)
		: m_menu(menu)
		, m_layout(createMenuLayout(menu.options().size()))
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

	const MenuSelection& MenuControl::selection() const
	{
		return m_selection;
	}
}
