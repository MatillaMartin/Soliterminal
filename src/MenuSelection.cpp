#include "MenuSelection.h"

namespace panda
{
	void MenuSelection::setIndex(size_t index) { m_index = index; }

	size_t MenuSelection::index() const { return m_index; }
}