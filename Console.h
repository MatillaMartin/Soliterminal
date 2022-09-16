#pragma once

#include <string>

namespace panda
{
	class Console
	{
	public:
		/// Creates a console abstraction
		Console();

		/// Starts drawing characters
		void begin();

		/// Ends drawing character
		void end();

		/// Clears the console
		/// Returns false if action failed
		bool clear();

		/// Returns the console width
		int width() const;

		/// Returns the console height
		int height() const;

		/// Draws the given string at row, column
		void draw(const std::string& str, int x, int y) const;

		/// Draws a white rectangle at row, column
		void drawRect(int x, int y, int width, int heigth) const;

		/// Draws a highlighted rectangle at row, column
		void drawRectHighlightGreen(int x, int y, int width, int heigth) const;

		/// Draws a highlighted rectangle at row, column
		void drawRectHighlightBlue(int x, int y, int width, int heigth) const;

		/// Draws the given string, with a shade under it
		void drawRectBottomShaded(int x, int y, int width, int heigth) const;

	private:
		bool setSize();
		bool setStyle();

		void setCursorPosition(int row, int column) const;

		void* m_handle = nullptr;
		int m_width = 0;
		int m_height = 0;
	};
}