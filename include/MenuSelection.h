#pragma once

namespace panda
{
	class MenuSelection
	{
	public:
		void setIndex(int index);
		int index() const;

	private:
		int m_index = 0;
	};
}