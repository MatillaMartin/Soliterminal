#pragma once

namespace panda
{
	class MenuSelection
	{
	public:
		void setIndex(size_t index);
		size_t index() const;

	private:
		size_t m_index;
	};
}