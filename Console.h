#pragma once

#include <string>
#include <vector>

namespace panda
{
	class Console
	{
	public:
		/// Creates a console abstraction
		Console();

		void setClearColor(int color);

		/// Starts drawing characters
		void begin();

		/// Ends drawing character
		void end();

		/// Returns the console width
		int width() const;

		/// Returns the console height
		int height() const;

		/// Sets the colors to be used in the next draw call
		void setDrawColor(int fgColor, int bgColor);

		/// Sets the colors to be used in the next draw call, uses clear color for background
		void setDrawColor(int fgColor);

		/// Draws the given string at row, column
		void draw(const std::string& str, int x, int y) const;

		/// Draws the given char at row, column
		void draw(char text, int x, int y) const;

		/// Draws a rectangle at row, column
		void drawRect(int x, int y, int width, int heigth) const;

		/// Draws a red rectangle with stripes
		void drawRectWithCrosses(int x, int y, int width, int heigth) const;

		/// Draw a rectangle outline
		void drawRectOutline(int x, int y, int width, int height, bool fill = true) const;

	private:
		// Helper method, prints the color number for foreground/backround colors
		void printColors() const;

		/// Clears the console
		/// Returns false if action failed
		bool clear(int color);

		// Sets up the color for the next draw call
		void setupColor() const;

		int color(int foreground, int background) const;

		void writeBuffer(const std::string& str) const;
		void writeBuffer(char c) const;

		bool setSize();
		bool setStyle();

		void setCursorPosition(int row, int column) const;

		void swapBuffers();

		void* m_firstBuffer = nullptr;
		void* m_secondBuffer = nullptr;
		void* m_backBuffer = nullptr;
		int m_width = 500;
		int m_height = 800;

		int m_fgColor = 0xF;
		int m_bgColor = 0x0;

		int m_clearColor = 0x0;
	};
}