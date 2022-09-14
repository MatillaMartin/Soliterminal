#include "Console.h"

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#endif

#include <assert.h>
#include <iostream>

namespace panda
{
	Console::Console()
	{
#ifdef WIN32

		// Get the Win32 handle representing standard output.
		// This generally only has to be done once, so we make it static.
		m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!m_handle)
			throw std::runtime_error("Couldn't access console");
#endif
		bool ok = updateSize();
		ok &= clear();
		if (!ok)
			throw std::runtime_error("Couldn't access console screen buffer info");
		assert(m_width > 0 && m_height > 0);
	}

	void Console::begin() { }

	void Console::end() { std::cout.flush(); }

	bool Console::clear()
	{
#ifdef WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		// Figure out the current width and height of the console window
		if (!GetConsoleScreenBufferInfo(m_handle, &csbi))
		{
			return false;
		}
		COORD topLeft = {0, 0};
		DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
		DWORD written;

		// Flood-fill the console with spaces to clear it
		FillConsoleOutputCharacter(m_handle, TEXT(' '), length, topLeft, &written);

		// Reset the attributes of every character to the default.
		// This clears all background colour formatting, if any.
		FillConsoleOutputAttribute(m_handle, csbi.wAttributes, length, topLeft, &written);

		// Move the cursor back to the top left for the next sequence of writes
		SetConsoleCursorPosition(m_handle, topLeft);
		return true;
#endif
		return false;
	}

	int Console::width() const { return m_width; }

	int Console::height() const { return m_height; }

	void Console::draw(const std::string& str, int row, int column) const
	{
		// Step through with a debugger, or insert sleeps, to see the effect.
		setCursorPosition(row, column);
		std::cout << str;
	}

	bool Console::updateSize()
	{
#ifdef WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		// Figure out the current width and height of the console window
		if (!GetConsoleScreenBufferInfo(m_handle, &csbi))
		{
			m_width = 0;
			m_height = 0;
			return false;
		}

		m_width = csbi.dwSize.X;
		m_height = csbi.dwSize.Y;
		return true;
#endif
		return false;
	}

	void Console::setCursorPosition(int row, int column) const
	{
		COORD coord = {(SHORT)row, (SHORT)column};
		SetConsoleCursorPosition(m_handle, coord);
	}
}