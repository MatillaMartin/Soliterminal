#include "MenuSelection.h"

namespace panda
{
	void MenuSelection::setIndex(int index) { m_index = index; }

	int MenuSelection::index() const { return m_index; }
}