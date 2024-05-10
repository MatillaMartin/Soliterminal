#pragma once

#include <utility>

namespace panda
{
	class Console;
	class Menu;
	class MenuSelection;
	class Layout;

	typedef std::pair<int, int> vec2i;

	class MenuRender
	{
	public:
		MenuRender(const Menu& menu, const MenuSelection& selection, const Layout& layout, Console& console);

		// Updates the rendering output
		void update();

	private:
		void drawControl(vec2i pos, int width);

		const Menu& m_menu;
		const MenuSelection& m_selection;
		const Layout& m_layout;
		Console& m_console;
	};
}