#pragma once
#include <string>
#include <vector>

namespace panda
{
	/// Menu with text and a series of buttons
	class Menu
	{
	public:
		Menu(std::string title, std::string text, std::vector<std::string> options)
			: m_title(std::move(title))
			, m_text(std::move(text))
			, m_options(std::move(options))
		{
		}

		const std::string& title() const { return m_title; }
		const std::string& text() const { return m_text; }
		const std::vector<std::string>& options() const { return m_options; }

	private:
		std::string m_title;
		std::string m_text;
		std::vector<std::string> m_options;
	};
}