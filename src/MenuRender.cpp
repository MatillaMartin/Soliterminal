#include "MenuRender.h"

#include "Console.h"
#include "Menu.h"
#include "MenuSelection.h"

#include <vector>

namespace panda
{
	static const int topMargin = 10;
	static const int leftMargin = 10;
	static const int ySubtitleSpacing = 2;
	static const int ySpacing = 5;
	static const int yOptSpacing = 3;

	int selectColor = 0xF;
	int markColor = 0xA;

	MenuRender::MenuRender(const Menu& menu, const MenuSelection& selection, const Layout& layout, Console& console)
		: m_menu(menu)
		, m_selection(selection)
		, m_layout(layout)
		, m_console(console)
	{
	}

	void MenuRender::update()
	{
		m_console.begin();

		int xLoc = topMargin;
		int yLoc = leftMargin;

		m_console.draw(m_menu.title(), xLoc, yLoc);

		yLoc++;
		for (int i = 0; i < 20; ++i)
			m_console.draw("=", xLoc + i, yLoc);

		yLoc += ySubtitleSpacing;
		m_console.draw(m_menu.text(), xLoc, yLoc);

		yLoc += ySpacing;

		std::vector<int> yOptLocs;
		for (const auto& opt : m_menu.optionNames())
		{
			m_console.draw(opt, xLoc, yLoc);
			yOptLocs.push_back(yLoc);
			yLoc += yOptSpacing;
		}

		// draw control on top
		int selectedOpt = m_selection.index();
		int optWidth = m_menu.optionNames().at(selectedOpt).size();
		drawControl({xLoc, yOptLocs[selectedOpt]}, optWidth);

		m_console.end();
	}

	void MenuRender::drawControl(vec2i pos, int width)
	{
		// draw control as arrows
		m_console.draw(char(16), pos.first - 1, pos.second);
		m_console.draw(char(17), pos.first + width, pos.second);
	}
}