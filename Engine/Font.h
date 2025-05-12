#pragma once

#include "Graphics.h"
#include "Surface.h"
#include "Vec2.h"

class Font
{
public:
	Font(const std::string& filename, Color chroma = Colors::White);
	void DrawText(const std::string& text, const Vei2& pos, Color color, Graphics& gfx, int textSpaceY = 0) const;
	void DrawTextCenter(const std::string& text, const Vei2& pos, Color color, Graphics& gfx, int textSpaceY = 0) const;
	int GetWidthChar() const;
	int GetHeightChar() const;
	static const int GetLongestLineSize(const std::string& text);
	static const int NumberOfLines(const std::string& text);
	RectI GetRectForText(const std::string& text, const Vei2& pos = Vei2{0,0}, int textSpaceY = 0) const;
private:
	RectI MapGlyphRect(char c) const;
private:
	// holds the font sheet bitmap data
	Surface surface;
	// this gives the dimensions of a glyph in the font sheet
	int glyphWidth;
	int glyphHeight;
	// number of rows / columns in the font sheet (this is fixed)
	static constexpr int nColumns = 32;
	static constexpr int nRows = 3;
	// font sheet chroma color
	Color chroma;
	// start and end drawable character codes
	static constexpr char firstChar = ' ';
	static constexpr char lastChar = '~';
};