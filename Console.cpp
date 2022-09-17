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
		: m_width(1500)
		, m_height(999)
	{
#ifdef WIN32
		// Get the Win32 handle representing standard output.
		// This generally only has to be done once, so we make it static.
		m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!m_handle)
			throw std::runtime_error("Could not console handle");
#endif
		bool ok = false;

		ok = setSize();
		if (!ok)
			throw std::runtime_error("Couldf not set console size");

		ok = setStyle();
		if (!ok)
			throw std::runtime_error("Could not set console style");

		ok = clear();
		if (!ok)
			throw std::runtime_error("Could not clear the console");
		assert(m_width > 0 && m_height > 0);
	}

	void Console::begin() { clear(); }

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
		FillConsoleOutputAttribute(m_handle, 0, length, topLeft, &written);

		// Move the cursor back to the top left for the next sequence of writes
		SetConsoleCursorPosition(m_handle, topLeft);
		return true;
#endif
		return false;
	}

	int Console::width() const { return m_width; }

	int Console::height() const { return m_height; }

	void Console::draw(const std::string& str, int x, int y) const
	{
		SetConsoleTextAttribute(m_handle, 240);
		setCursorPosition(x, y);
		std::cout << str;
	}

	void Console::draw(char text, int x, int y) const
	{
		SetConsoleTextAttribute(m_handle, 0x0F);
		setCursorPosition(x, y);
		std::cout << text;
	}

	void Console::drawRect(int x, int y, int width, int height) const
	{
		SetConsoleTextAttribute(m_handle, 240);
		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				// draw all empty spaces
				setCursorPosition(x + i, y + j);
				std::cout << " ";
			}
		}
	}

	void Console::drawRectBottomShaded(int x, int y, int width, int height) const
	{
		if (width == 0 || height == 0)
			return;

		drawRect(x, y, width, height);

		// draw last line with char 95 and color to add a shade effect
		for (int j = 0; j < width; ++j)
		{
			setCursorPosition(x + j, y + height - 1);
			std::cout << "\u001b[38;5;250m" << char(95);
		}
	}

	void Console::drawRectRedFancy(int x, int y, int width, int height) const
	{
		drawCrosses(x, y, width, height, 0xCF);
	}

	void Console::drawRectRedFancyShaded(int x, int y, int width, int height) const
	{
		drawRectRedFancy(x, y, width, height);

		// draw last line with char 95 and color to add a shade effect
		for (int j = 0; j < width; ++j)
		{
			setCursorPosition(x + j, y + height - 1);
			SetConsoleTextAttribute(m_handle, 0xC4);
			std::cout << char(95);
		}
	}

	void Console::drawRectOutline(int x, int y, int width, int height, int color, bool fill) const
	{
		SetConsoleTextAttribute(m_handle, color);
		// draw edges out of loop
		// top left 218: ┌
		setCursorPosition(x, y);
		std::cout << char(218);

		// top right 191: ┐
		setCursorPosition(x + width - 1, y);
		std::cout << char(191);

		// bottom left 192: └
		setCursorPosition(x, y + height - 1);
		std::cout << char(192);

		// bottom right 217: ┘
		setCursorPosition(x + width - 1, y + height - 1);
		std::cout << char(217);

		// draw top and bottom edges 196: ─
		for (int i = 1; i < width - 1; ++i)
		{
			setCursorPosition(x + i, y);
			std::cout << char(196);
			setCursorPosition(x + i, y + height - 1);
			std::cout << char(196);
		}

		// draw left and right edges 179: │
		for (int j = 1; j < height - 1; ++j)
		{
			setCursorPosition(x, y + j);
			std::cout << char(179);
			setCursorPosition(x + width - 1, y + j);
			std::cout << char(179);
		}

		if (fill)
		{
			for (int i = 1; i < width - 1; ++i)
			{
				for (int j = 1; j < height - 1; ++j)
				{
					setCursorPosition(x + i, y + j);
					std::cout << " ";
				}
			}
		}
	}

	void Console::drawCrosses(int x, int y, int width, int height, int color) const
	{
		SetConsoleTextAttribute(m_handle, color);
		// draw top and bottom edges 205: ═
		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				setCursorPosition(x + i, y + j);
				std::cout << " ";
			}
		}
		
		// draw crosses in the middle
		for (int i = 1; i < width - 1; ++i)
		{
			for (int j = 1; j < height - 1; ++j)
			{
				setCursorPosition(x+ i, y + j);
				std::cout << char(206);
			}
		}
	}

	bool Console::setSize()
	{
#ifdef WIN32
		HWND consoleWindow = GetConsoleWindow();

		// Place the window, set size
		MoveWindow(consoleWindow, 100, 10, m_width, m_height, TRUE);

		// Adjust buffer to match console size
		CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;

		if (!GetConsoleScreenBufferInfo(m_handle, &scrBufferInfo))
			return false;

		// current window size
		short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
		short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;

		// current screen buffer size
		short scrBufferWidth = scrBufferInfo.dwSize.X;
		short scrBufferHeight = scrBufferInfo.dwSize.Y;

		// to remove the scrollbar, make sure the window height matches the screen buffer height
		COORD newSize;
		newSize.X = scrBufferWidth;
		newSize.Y = winHeight;

		// set the new screen buffer dimensions
		if (SetConsoleScreenBufferSize(m_handle, newSize) == 0)
			return false;
		return true;
#endif
		return false;
	}

	bool Console::setStyle()
	{
#ifdef WIN32
		HWND consoleWindow = GetConsoleWindow();
		LONG lStyle = GetWindowLong(consoleWindow, GWL_STYLE);
		lStyle &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX);
		SetWindowLong(consoleWindow, GWL_STYLE, lStyle);

		// Hide cursor
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(m_handle, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(m_handle, &cursorInfo);
		return true;
#endif
		return false;
	}

	void Console::setCursorPosition(int x, int y) const
	{
		COORD coord = {(SHORT)x, (SHORT)y};
		SetConsoleCursorPosition(m_handle, coord);
	}
}