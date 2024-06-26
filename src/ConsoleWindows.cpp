﻿#include "ConsoleWindows.h"

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#endif

#include <assert.h>
#include <iostream>
#include <sstream>

namespace panda
{
	namespace
	{
		void printLastError(const std::string_view& context)
		{
			int error = GetLastError();
			std::string message = std::system_category().message(error);
			std::cout << context << ": " << message << std::endl;
		}
	}

	ConsoleWindows::ConsoleWindows()
	{
		SetConsoleTitle("Soliterminal");

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

	ConsoleWindows::~ConsoleWindows()
	{
		if (m_secondBuffer)
			CloseHandle(m_secondBuffer);
	}

	void ConsoleWindows::setClearColor(int color) { m_clearColor = color; }

	void ConsoleWindows::begin() { clear(m_clearColor); }

	void ConsoleWindows::end()
	{
		//printColors();
		//printAscii();
		swapBuffers();
	}

	bool ConsoleWindows::clear(int color)
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

	int ConsoleWindows::width() const { return m_width; }

	int ConsoleWindows::height() const { return m_height; }

	void ConsoleWindows::setDrawColor(int fgColor, int bgColor)
	{
		m_fgColor = fgColor;
		m_bgColor = bgColor;
	}

	void ConsoleWindows::setDrawColor(int fgColor) { setDrawColor(fgColor, m_clearColor); }

	void ConsoleWindows::draw(const std::string& str, int x, int y) const
	{
		setupColor();
		setCursorPosition(x, y);
		writeBuffer(str);
	}

	void ConsoleWindows::draw(char text, int x, int y) const
	{
		setupColor();
		setCursorPosition(x, y);
		writeBuffer(text);
	}

	void ConsoleWindows::drawRect(int x, int y, int width, int height) const
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

	void ConsoleWindows::drawRectOutline(int x, int y, int width, int height, bool fill) const
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

	void ConsoleWindows::clear() { system("cls"); }

	void ConsoleWindows::printColors() const
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

	void ConsoleWindows::printAscii() const
	{
		for (int i = 0; i < 256; ++i)
		{
			writeBuffer(std::to_string(i) + ": ");
			writeBuffer(char(i));
			writeBuffer("\t");
		}
	}

	void ConsoleWindows::setupColor() const { SetConsoleTextAttribute(m_backBuffer, color(m_fgColor, m_bgColor)); }

	int ConsoleWindows::color(int foreground, int background) const { return foreground + background * 16; }

	void ConsoleWindows::writeBuffer(const std::string& str) const
	{
		// write to back buffer
		DWORD written;
		WriteConsole(m_backBuffer, str.c_str(), static_cast<int>(str.length()), &written, nullptr);
	}

	void ConsoleWindows::writeBuffer(char c) const
	{
		DWORD written;
		WriteConsole(m_backBuffer, &c, 1, &written, nullptr);
	}

	bool ConsoleWindows::setSize()
	{
		/// Set buffer to match size so there are no scrollbars
		// Retrieve screen buffer info
		CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
		bool ok = GetConsoleScreenBufferInfo(m_firstBuffer, &scrBufferInfo);
		if (!ok)
		{
			printLastError("GetConsoleScreenBufferInfo");
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
			{
				printLastError("SetConsoleScreenBufferSize first buffer");
				return false;
			}
		}
		{
			bool ok = SetConsoleScreenBufferSize(m_secondBuffer, newSize);
			if (!ok)
			{
				printLastError("SetConsoleScreenBufferSize second buffer");
				return false;
			}
		}

		SMALL_RECT windowInfo = {0, 0, newSize.X - 1, newSize.Y - 1};
		{
			bool ok = SetConsoleWindowInfo(m_firstBuffer, TRUE, &windowInfo);
			if (!ok)
			{
				printLastError("SetConsoleWindowInfo first buffer");
				return false;
			}
		}
		{
			bool ok = SetConsoleWindowInfo(m_secondBuffer, TRUE, &windowInfo);
			if (!ok)
			{
				printLastError("SetConsoleWindowInfo second buffer");
				return false;
			}
		}

		// Set console window position and size for both buffers
		RECT rect = {m_topLeft.first, m_topLeft.second, m_width + m_topLeft.first, m_height + m_topLeft.second};
		{
			HWND hwnd = GetConsoleWindow();
			bool ok = MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
			if (!ok)
			{
				printLastError("GetConsoleWindow");
				return false;
			}
		}
		swapBuffers();
		{
			HWND hwnd = GetConsoleWindow();
			bool ok = MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
			if (!ok)
			{
				printLastError("GetConsoleWindow");
				return false;
			}
		}
		swapBuffers();

		// reset a third time because reasons.. seems to fix the issue with one of the buffers not being the correct size
		{
			HWND hwnd = GetConsoleWindow();
			bool ok = MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
			if (!ok)
			{
				printLastError("GetConsoleWindow");
				return false;
			}
		}
		swapBuffers();

		return true;
	}

	bool ConsoleWindows::setStyle()
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

	void ConsoleWindows::setCursorPosition(int x, int y) const
	{
		COORD coord = {(SHORT)x, (SHORT)y};
		SetConsoleCursorPosition(m_backBuffer, coord);
	}

	void ConsoleWindows::swapBuffers()
	{
		bool ok = SetConsoleActiveScreenBuffer(m_backBuffer);
		if (!ok)
		{
			int error = GetLastError();
			std::string message = std::system_category().message(error);
			std::cout << message << std::endl;
			throw std::runtime_error("Could not swap buffers");
		}

		m_backBuffer = m_backBuffer == m_firstBuffer ? m_secondBuffer : m_firstBuffer;
	}
}