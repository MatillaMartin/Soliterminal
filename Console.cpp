#include "Console.h"

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#endif

#include <assert.h>
#include <iostream>
#include <sstream>

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

		assert(m_width > 0 && m_height > 0);
	}

	void Console::setClearColor(int color) { m_clearColor = color; }

	void Console::begin() { clear(m_clearColor); }

	void Console::end()
	{
		//printColors();
		//printAscii();
		swapBuffers();
	}

	bool Console::clear(int color)
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
		FillConsoleOutputCharacter(m_backBuffer, ' ', length, topLeft, &written);

		WORD attribute;
		attribute = this->color(0, color);    // change the background color
		FillConsoleOutputAttribute(m_backBuffer, attribute, length, topLeft, &written);


		// Move the cursor back to the top left for the next sequence of writes
		SetConsoleCursorPosition(m_backBuffer, topLeft);
		return true;
	}

	int Console::width() const { return m_width; }

	int Console::height() const { return m_height; }

	void Console::setDrawColor(int fgColor, int bgColor)
	{
		m_fgColor = fgColor;
		m_bgColor = bgColor;
	}

	void Console::setDrawColor(int fgColor) { setDrawColor(fgColor, m_clearColor); }

	void Console::draw(const std::string& str, int x, int y) const
	{
		setupColor();
		setCursorPosition(x, y);
		writeBuffer(str);
	}

	void Console::draw(char text, int x, int y) const
	{
		setupColor();
		setCursorPosition(x, y);
		writeBuffer(text);
	}

	void Console::drawRect(int x, int y, int width, int height) const
	{
		setupColor();
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

	void Console::drawRectWithCrosses(int x, int y, int width, int height) const
	{
		setupColor();
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

	void Console::drawRectOutline(int x, int y, int width, int height, bool fill) const
	{
		setupColor();
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

	void Console::printColors() const
	{
		for (int i = 0; i < 0xFF; ++i)
		{
			SetConsoleTextAttribute(m_backBuffer, i);
			std::stringstream stream;
			stream << std::hex << i;
			//writeBuffer(stream.str());
			writeBuffer(std::to_string(i % 16));
		}
	}

	void Console::printAscii() const
	{
		for (int i = 0; i < 256; ++i)
		{
			writeBuffer(std::to_string(i) + ": ");
			writeBuffer(char(i));
			writeBuffer("\t");
		}
	}

	void Console::setupColor() const { SetConsoleTextAttribute(m_backBuffer, color(m_fgColor, m_bgColor)); }

	int Console::color(int foreground, int background) const { return foreground + background * 16; }

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
		RECT rect = {m_topLeft.first, m_topLeft.second, m_width + m_topLeft.first, m_height + m_topLeft.second};
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