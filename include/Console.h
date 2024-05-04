#pragma once

#include <string>
#include <vector>

namespace panda
{
	class Console
	{
	public:
		/// Sets the color to clear the screen with
		virtual void setClearColor(int color) = 0;

		/// Starts drawing characters
		virtual void begin() = 0;

		/// Ends drawing character
		virtual void end() = 0;

		/// Returns the console width
		virtual int width() const = 0;

		/// Returns the console height
		virtual int height() const = 0;

		/// Sets the colors to be used in the next draw call
		virtual void setDrawColor(int fgColor, int bgColor) = 0;

		/// Sets the colors to be used in the next draw call, uses clear color for background
		virtual void setDrawColor(int fgColor) = 0;

		/// Draws the given string at row, column
		virtual void draw(const std::string& str, int x, int y) const = 0;

		/// Draws the given char at row, column
		virtual void draw(char text, int x, int y) const = 0;

		/// Draws a rectangle at row, column
		virtual void drawRect(int x, int y, int width, int heigth) const = 0;

		/// Draw a rectangle outline
		virtual void drawRectOutline(int x, int y, int width, int height, bool fill = true) const = 0;

		/// Clears the console from all output
		virtual void clear() = 0;
	};
}