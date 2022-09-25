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

		/// Sets the color with which the background is cleared
		void setBackgroundColor(int color);

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

		/// Draws the given char at row, column
		void draw(char text, int x, int y) const;

		/// Draws a white rectangle at row, column
		void drawRect(int x, int y, int width, int heigth) const;

		/// Draws the given string, with a shade under it
		void drawRectShaded(int x, int y, int width, int heigth) const;

		/// Draws a red rectangle with stripes
		void drawRectRedWithCrosses(int x, int y, int width, int heigth) const;

		/// Draws a red rectangle with stripes, with a shade under it
		void drawRectRedWithCrossesShaded(int x, int y, int width, int heigth) const;

		/// Draw a rectangle outline
		void drawRectOutline(int x, int y, int width, int height, int color = 0x0F, bool fill = true) const;

	private:
		// Helper method, prints the color number for foreground/backround colors
		void printColors() const;

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
		int m_bgColor = 0x00;
	};
}