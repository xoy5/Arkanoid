/******************************************************************************************
 *	InterfaceObject.h																	  *
 *  Info for all                                                                          *
 *  Header Files > Framework > *.h														  *
 *	Kacper Wojtkowski December 2024 r.												      *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 *****************************************************************************************/

 // !!!!!!!!!!!!!!!!!! IMPORTANT  !!!!!!!!!!!!!!!!!!!!!  
 // > Use `box-sizing: content-box;` (for sizeWidth and sizeHeight).  
 // > To be honest there is not sense to make border-box
 // > There's no need to set up the button during creation.  
 // > `MenuButton` is a type of button that must be placed  
 //   within a class that contains an `enum class`.  
 // 
 // 
 // > SetText(...) updates size u can change it by SetDynamicSize(false);
 // > box-sizing: content-box;
 // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  

#pragma once
#include "Font.h"
#include "Vec2.h"
#include "Sound.h"
#include "Mouse.h"
#include "Graphics.h"

class InterfaceObject
{
public:
	InterfaceObject(const Font* font, const std::string& text, const Vei2& pos)
		:
		font(font),
		text(text),
		pos(pos),
		sizeWidth((int)text.size()* font->GetWidthChar()),
		sizeHeight(font->GetHeightChar())
	{}
	void Draw(Graphics& gfx) const
	{
		const int borderSize = isSetBorder ? this->borderSize : 0;

		// border
		if (isSetBorder) {
			if (active) {
				gfx.DrawRect(GetRect(), borderColorActive);
			}
			else if (hovered) {
				gfx.DrawRect(GetRect(), borderColorHovered);
			}
			else {
				gfx.DrawRect(GetRect(), borderColor);
			}
		}

		// background
		if (isSetBackground && isSetBackgroundHoverDarker && hovered) {
			gfx.DrawRect(GetRect().GetExpanded(-borderSize), Color::GetDarker(backgroundColor, darker));
		}
		else if (isSetBackground) {
			gfx.DrawRect(GetRect().GetExpanded(-borderSize), backgroundColor);
		}

		//////// text drawing ////////
		Vei2 textPos = pos + Vei2{ borderSize + paddingX, borderSize + paddingY };
		// positionCenter
		if (isSetPositionCenter){
			textPos -= Vei2{ sizeWidth / 2 + borderSize + paddingX, sizeHeight / 2 + borderSize + paddingY };
		}
		// textAlignCenter
		if (isSetTextAlignCenter) {
			const int textWidth = (int)text.size() * font->GetWidthChar();
			textPos += Vei2{ (sizeWidth - textWidth) / 2, (sizeHeight - font->GetHeightChar()) / 2 };
		}
		font->DrawText(text, textPos, fontColor, gfx);
		///////////////////////////////
		if (isDisabled) {
			gfx.DrawDisabled(GetRect());
		}
	}
public:
	void SetActive(bool set = false)
	{
		this->active = set;
	}
	RectI GetRect() const
	{
		const int borderSize = isSetBorder ? this->borderSize : 0;
		if (isSetPositionCenter == false) return RectI(pos, sizeWidth + paddingX * 2 + borderSize * 2, sizeHeight + paddingY * 2 + borderSize * 2);
		else return RectI::FromCenter(pos, sizeWidth / 2 + paddingX + borderSize, sizeHeight / 2 + paddingY + borderSize);
	}
	void SetPos(const Vei2& pos)
	{
		this->pos = pos;
	}
	Vei2 GetPos() const
	{
		return pos;
	}
	int GetHeight() const
	{
		return paddingY * 2 + borderSize * 2 + font->GetHeightChar();
	}
	virtual void ProcessMouse(const Mouse::Event& event)
	{
		// hovered
		hovered = GetRect().IsContains(event.GetPos()) && isDisabled == false;

		if (hovered && !hoveredAlready) {
			hoverSound.Play();
			hoveredAlready = true;
		}
		else if (!hovered) {
			hoveredAlready = false;
		}

		// clicked
		if (hovered) {
			if (event.GetType() == Mouse::Event::Type::LPress) {
				clickedIn = true;
			}

			clicked = (clickedIn && event.GetType() == Mouse::Event::Type::LRelease);

			if (clickedIn && event.LeftIsPressed()) {
				active = true;
			}
			else {
				active = false;
			}
		}
		else {
			active = false;
			clickedIn = false;
		}
	}
	bool IsClicked() const
	{
		return hovered && clicked && isDisabled == false;
	}
	bool IsHovered() const
	{
		return hovered;
	}
	void SetText(const std::string& text)
	{
		this->text = text;
		if (isSetDynamicSize) {
			sizeWidth = (int)text.size() * font->GetWidthChar();
		}
	}
	std::string GetText() const
	{
		return text;
	}
	void SetSizeWidthContentBox(int width)
	{
		this->sizeWidth = width;
	}
	void SetSizeHeightContentBox(int height)
	{
		this->sizeHeight = height;
	}
	void SetSizeWidthBorderBox(int width)
	{
		this->sizeWidth = width - (paddingX * 2) - (borderSize * 2);
	}
	void SetSizeHeightBorderBox(int height)
	{
		this->sizeHeight = height - (paddingY * 2) - (borderSize * 2);
	}

public:
	//************* STYLE DEFAULTS *************//
	// naked
	void ToNaked()
	{
		// dynamic size
		isSetDynamicSize = false;
		// font
		fontColor = Colors::White;
		isSetFontHoverDarker = false;
		// text
		isSetTextAlignCenter = true;
		// border
		isSetBorder = false;
		// background
		isSetBackground = false;
	}
	// default
	void ToDefault()
	{
		// dynamic size
		isSetDynamicSize = true;
		// darker
		darker = 20;
		// position
		isSetPositionCenter = false;
		// font
		fontColor = Colors::Black;
		isSetFontHoverDarker = true;
		// text
		isSetTextAlignCenter = true;
		// border
		isSetBorder = true;
		borderColor = Colors::Gray;
		borderSize = 5;
		// background
		isSetBackground = true;
		backgroundColor = Colors::White;
		isSetBackgroundHoverDarker = true;
	}
	//*********** SETTERS FOR STYLES ***********//
	// disabled
	void SetDisabled(bool set = false)
	{
		this->isDisabled = set;
	}
	// dynamic size
	void SetDynamicSize(bool set = true)
	{
		this->isSetDynamicSize = set;
	}
	// darker
	void SetDarker(int valDarker)
	{
		this->darker = valDarker;
	}
	// position
	void SetPositionCenter(bool set = false)
	{
		this->isSetPositionCenter = set;
	}
	// font
	void SetFontHoverDarker(bool set = true)
	{
		this->isSetFontHoverDarker = set;
	}
	void SetFontColor(const Color& color)
	{
		this->fontColor = color;
	}
	// text
	void SetTextAlignCenter(bool set = true)
	{
		this->isSetTextAlignCenter = set;
	}
	// size
	void SetSize(int width, int height)
	{
		this->sizeWidth = width > font->GetWidthChar() * text.size() ? width : font->GetWidthChar() * (int)text.size();
		this->sizeHeight = height > font->GetHeightChar() ? height : font->GetHeightChar();
	}
	// padding
	void SetPadding(int paddingX = 12, int paddingY = 8)
	{
		this->paddingX = paddingX;
		this->paddingY = paddingY;
	}
	// border
	void SetBorder(bool set = true, int size = 5, Color color = Colors::Gray, Color colorHovered = Colors::Blue, Color colorActive = Colors::Green)
	{
		this->isSetBorder = set;
		this->borderSize = size;
		this->borderColor = color;
		this->borderColorHovered = colorHovered;
		this->borderColorActive = colorActive;
	}
	// background
	void SetBackground(bool set = true, Color color = Colors::White, bool setHoverDarker = true)
	{
		this->isSetBackground = set;
		this->backgroundColor = color;
		this->isSetBackgroundHoverDarker = setHoverDarker;
	}
	//********************************//

protected:
	const Font* font;
	Vei2 pos;
	std::string text;
	bool hovered = false;
	bool clicked = false;
	bool clickedIn = false;
	bool active = false;
	Sound hoverSound = { L"Files/Sounds/menu_boop.wav" };
	bool hoveredAlready = false;

	//*********** STYLES ***********//
	// disabled
	bool isDisabled = false;
	// dynamic size
	bool isSetDynamicSize = true;
	// darker
	int darker = 20;
	// position
	bool isSetPositionCenter = false;
	// font
	Color fontColor = Colors::Black;
	bool isSetFontHoverDarker = true;
	// text
	bool isSetTextAlignCenter = true;
	// size (width and height are set according to the font size)
	int sizeWidth;
	int sizeHeight;
	// padding
	int paddingX = 12;
	int paddingY = 8;
	// border
	bool isSetBorder = true;
	Color borderColor = Colors::Gray;
	Color borderColorHovered = Colors::Blue;
	Color borderColorActive = Colors::Green;
	int borderSize = 5;
	// background
	bool isSetBackground = true;
	Color backgroundColor = Colors::White;
	bool isSetBackgroundHoverDarker = true;
	//*****************************//
};