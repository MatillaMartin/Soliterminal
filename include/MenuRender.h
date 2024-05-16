#pragma once
#include "Render.h"

#include <utility>

namespace panda
{
	class Console;
	class Menu;
	class MenuSelection;

	typedef std::pair<int, int> vec2i;

	class MenuRender : public Render
	{
	public:
		MenuRender(const Menu& menu, const MenuSelection& selection, Console& console);

		// Updates the rendering output
		void update();

	private:
		void drawControl(vec2i pos, int width);

		const Menu& m_menu;
		const MenuSelection& m_selection;
		Console& m_console;
	};
}