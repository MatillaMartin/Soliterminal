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
		SetConsoleTitle("Console Solitaire");

		// Get the intial console buffer.
		m_firstBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
		// Create an additional buffer for switching.
		m_secondBuffer =
			CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
		// Initialize back buffer
		m_backBuffer = m_secondBuffer;

		if (!m_backBuffer)
			throw std::runtime_error("Could not console handle");
		bool ok = false;

		ok = setSize();
		if (!ok)
			throw std::runtime_error("Could not set console size");

		ok = setStyle();
		if (!ok)
			throw std::runtime_error("Could not set console style");

		ok = clear();
		if (!ok)
			throw std::runtime_error("Could not clear the console");
		assert(m_width > 0 && m_height > 0);
	}

	void Console::begin() { clear(); }

	void Console::end() { swapBuffers(); }

	bool Console::clear()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		// Figure out the current width and height of the console window
		if (!GetConsoleScreenBufferInfo(m_backBuffer, &csbi))
		{
			return false;
		}
		COORD topLeft = {0, 0};
		DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
		DWORD written;

		// Flood-fill the console with spaces to clear it
		FillConsoleOutputCharacter(m_backBuffer, TEXT(' '), length, topLeft, &written);

		// Reset the attributes of every character to the default.
		// This clears all background colour formatting, if any.
		FillConsoleOutputAttribute(m_backBuffer, 0, length, topLeft, &written);

		// Move the cursor back to the top left for the next sequence of writes
		SetConsoleCursorPosition(m_backBuffer, topLeft);
		return true;
	}

	int Console::width() const { return m_width; }

	int Console::height() const { return m_height; }

	void Console::draw(const std::string& str, int x, int y) const
	{
		SetConsoleTextAttribute(m_backBuffer, 240);
		setCursorPosition(x, y);
		writeBuffer(str);
	}

	void Console::draw(char text, int x, int y) const
	{
		SetConsoleTextAttribute(m_backBuffer, 0x0F);
		setCursorPosition(x, y);
		writeBuffer(text);
	}

	void Console::drawRect(int x, int y, int width, int height) const
	{
		SetConsoleTextAttribute(m_backBuffer, 240);
		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				// draw all empty spaces
				setCursorPosition(x + i, y + j);
				writeBuffer(" ");
			}
		}
	}

	void Console::drawRectShaded(int x, int y, int width, int height) const
	{
		if (width == 0 || height == 0)
			return;

		drawRect(x, y, width, height);

		// draw last line with char 95 '_'
		SetConsoleTextAttribute(m_backBuffer, 0xF8);
		for (int j = 0; j < width; ++j)
		{
			setCursorPosition(x + j, y + height - 1);
			writeBuffer(char(95));
		}
	}

	void Console::drawRectRedWithCrosses(int x, int y, int width, int height) const
	{
		SetConsoleTextAttribute(m_backBuffer, 0xCF);
		// draw top and bottom edges 205: ═
		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				setCursorPosition(x + i, y + j);
				writeBuffer(" ");
			}
		}

		// draw crosses in the middle
		for (int i = 1; i < width - 1; ++i)
		{
			for (int j = 1; j < height - 1; ++j)
			{
				setCursorPosition(x + i, y + j);
				writeBuffer(char(206));
			}
		}
	}

	void Console::drawRectRedWithCrossesShaded(int x, int y, int width, int height) const
	{
		drawRectRedWithCrosses(x, y, width, height);

		// draw last line with char 95 and color to add a shade effect
		for (int j = 0; j < width; ++j)
		{
			setCursorPosition(x + j, y + height - 1);
			SetConsoleTextAttribute(m_backBuffer, 0xC4);
			writeBuffer(char(95));
		}
	}

	void Console::drawRectOutline(int x, int y, int width, int height, int color, bool fill) const
	{
		SetConsoleTextAttribute(m_backBuffer, color);
		// draw edges out of loop
		// top left 218: ┌
		setCursorPosition(x, y);
		writeBuffer(char(218));

		// top right 191: ┐
		setCursorPosition(x + width - 1, y);
		writeBuffer(char(191));

		// bottom left 192: └
		setCursorPosition(x, y + height - 1);
		writeBuffer(char(192));

		// bottom right 217: ┘
		setCursorPosition(x + width - 1, y + height - 1);
		writeBuffer(char(217));

		// draw top and bottom edges 196: ─
		for (int i = 1; i < width - 1; ++i)
		{
			setCursorPosition(x + i, y);
			writeBuffer(char(196));
			setCursorPosition(x + i, y + height - 1);
			writeBuffer(char(196));
		}

		// draw left and right edges 179: │
		for (int j = 1; j < height - 1; ++j)
		{
			setCursorPosition(x, y + j);
			writeBuffer(char(179));
			setCursorPosition(x + width - 1, y + j);
			writeBuffer(char(179));
		}

		if (fill)
		{
			for (int i = 1; i < width - 1; ++i)
			{
				for (int j = 1; j < height - 1; ++j)
				{
					setCursorPosition(x + i, y + j);
					writeBuffer(" ");
				}
			}
		}
	}

	void Console::writeBuffer(const std::string& str) const
	{
		// write to back buffer
		DWORD written;
		WriteConsole(m_backBuffer, str.c_str(), str.length(), &written, nullptr);
	}

	void Console::writeBuffer(char c) const
	{
		DWORD written;
		WriteConsole(m_backBuffer, &c, 1, &written, nullptr);
	}

	bool Console::setSize()
	{
		/// Set buffer to match size so there are no scrollbars
		// Retrieve screen buffer info
		CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
		bool ok = GetConsoleScreenBufferInfo(m_firstBuffer, &scrBufferInfo);
		if (!ok)
		{
			int error = GetLastError();
			std::string message = std::system_category().message(error);
			std::cout << message << std::endl;
			return false;
		}

		// current window and buffer size
		short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
		short scrBufferWidth = scrBufferInfo.dwSize.X;

		// to remove the scrollbar, make sure the window height matches the screen buffer height
		COORD newSize;
		newSize.X = scrBufferWidth;
		newSize.Y = winHeight;

		//Change the internal buffer size:
		{
			bool ok = SetConsoleScreenBufferSize(m_firstBuffer, newSize);
			if (!ok)
				return false;
		}
		{
			bool ok = SetConsoleScreenBufferSize(m_secondBuffer, newSize);
			if (!ok)
				return false;
		}

		SMALL_RECT windowInfo = {0, 0, newSize.X - 1, newSize.Y - 1};
		{
			bool ok = SetConsoleWindowInfo(m_firstBuffer, TRUE, &windowInfo);
			if (!ok)
				return false;
		}
		{
			bool ok = SetConsoleWindowInfo(m_secondBuffer, TRUE, &windowInfo);
			if (!ok)
				return false;
		}

		// Set console window position and size for both buffers
		RECT rect = {100, 100, m_width + 100, m_height + 100};
		{
			HWND hwnd = GetConsoleWindow();
			MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		}
		swapBuffers();
		{
			HWND hwnd = GetConsoleWindow();
			MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		}
		swapBuffers();    // reset state

		return true;
	}

	bool Console::setStyle()
	{
		HWND consoleWindow = GetConsoleWindow();
		LONG lStyle = GetWindowLong(consoleWindow, GWL_STYLE);
		lStyle &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX);
		SetWindowLong(consoleWindow, GWL_STYLE, lStyle);

		// Hide cursor in both buffers
		auto hideCursor = [](void* handle) {
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(handle, &cursorInfo);
			cursorInfo.bVisible = false;
			SetConsoleCursorInfo(handle, &cursorInfo);
		};
		hideCursor(m_firstBuffer);
		hideCursor(m_secondBuffer);
		return true;
	}

	void Console::setCursorPosition(int x, int y) const
	{
		COORD coord = {(SHORT)x, (SHORT)y};
		SetConsoleCursorPosition(m_backBuffer, coord);
	}

	void Console::swapBuffers()
	{
		SetConsoleActiveScreenBuffer(m_backBuffer);
		m_backBuffer = m_backBuffer == m_firstBuffer ? m_secondBuffer : m_firstBuffer;
	}
}