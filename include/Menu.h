#pragma once
#include <algorithm>
#include <functional>
#include <string>
#include <vector>

namespace panda
{
	typedef std::pair<std::string, std::function<void()>> Option;

	/// Menu with text and a series of buttons
	class Menu
	{
	public:
		Menu(std::string title, std::string text, std::vector<Option> options)
			: m_title(std::move(title))
			, m_text(std::move(text))
			, m_options(std::move(options))
		{
		}

		const std::string& title() const { return m_title; }
		const std::string& text() const { return m_text; }
		const std::vector<std::string> optionNames() const
		{
			std::vector<std::string> transformed;
			transformed.resize(m_options.size());
			std::transform(m_options.begin(), m_options.end(), transformed.begin(), [](const Option& opt) { return opt.first; });
			return transformed;
		}

		const std::vector<Option>& options() const { return m_options; }

	private:
		std::string m_title;
		std::string m_text;
		std::vector<Option> m_options;
	};
}