#include "Font.h"
#include <cassert>
#include "SpriteEffect.h"

Font::Font( const std::string& filename,Color chroma )
	:
	surface( filename ),
	// calculate glyph dimensions from bitmap dimensions
	glyphWidth( surface.GetWidth() / nColumns ),
	glyphHeight( surface.GetHeight() / nRows ),
	chroma( chroma )
{
	// verify that bitmap had valid dimensions
	assert( glyphWidth * nColumns == surface.GetWidth() );
	assert( glyphHeight * nRows == surface.GetHeight() );
}

void Font::DrawText( const std::string& text,const Vei2& pos,Color color,Graphics& gfx, int textSpaceY) const
{
	// create effect functor
	SpriteEffect::Substitution e{ chroma,color };
	// curPos is the pos that we are drawing to on the screen
	auto curPos = pos;
	for( auto c : text )
	{
		// on a newline character, reset x position and move down by 1 glyph height
		if( c == '\n' )
		{
			// carriage return
			curPos.x = pos.x;
			// line feed
			curPos.y += glyphHeight + textSpaceY;
			// we don't want to advance the character position right for a newline
			continue;
		}
		// only draw characters that are on the font sheet
		// start at firstChar + 1 because might as well skip ' ' as well
		else if( c >= firstChar + 1 && c <= lastChar )
		{
			// use DrawSpriteSubstitute so that we can choose the color of the font rendered
			gfx.DrawSprite( curPos.x,curPos.y,MapGlyphRect( c ),surface,e );
		}
		// advance screen pos for next character
		curPos.x += glyphWidth;
	}
}

void Font::DrawTextCenter(const std::string& text, const Vei2& pos, Color color, Graphics& gfx, int textSpaceY) const
{
	DrawText(text, pos - GetRectForText(text).GetSizes() / 2, color, gfx, textSpaceY);
}

int Font::GetWidthChar() const
{
	return surface.GetWidth() / nColumns;
}

int Font::GetHeightChar() const
{
	return surface.GetHeight() / nRows;
}

const int Font::GetLongestLineSize(const std::string& text)
{
	int maxLength = 0;
	int currentLength = 0;

	for (char c : text)
	{
		if (c == '\n')
		{
			if (currentLength > maxLength) maxLength = currentLength;
			currentLength = 0;
		}
		else ++currentLength;
	}

	if (currentLength > maxLength) maxLength = currentLength;

	return maxLength;
}

const int Font::NumberOfLines(const std::string& text)
{
	int n = 0;
	for (char c : text)
	{
		if (c == '\n') n++;
	}
	return n + 1;
}

RectI Font::GetRectForText(const std::string& text, const Vei2& pos, int textSpaceY) const
{
	return RectI(pos, glyphWidth * GetLongestLineSize(text), glyphHeight * NumberOfLines(text));
}

RectI Font::MapGlyphRect( char c ) const
{
	assert( c >= firstChar && c <= lastChar );
	// font sheet glyphs start at ' ', calculate index into sheet
	const int glyphIndex = c - ' ';
	// map 1d glyphIndex to 2D coordinates
	const int yGlyph = glyphIndex / nColumns;
	const int xGlyph = glyphIndex % nColumns;
	// convert the sheet grid coords to pixel coords in sheet
	return RectI(
		{ xGlyph * glyphWidth,yGlyph * glyphHeight },
		glyphWidth,glyphHeight
	);
}
