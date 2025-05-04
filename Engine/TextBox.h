#pragma once
#include <string>
#include <vector>

#include "InterfaceObject.h"
#include "Mouse.h"
#include "Vec2.h"
#include "Font.h"
     

class TextBox : public InterfaceObject
{
public:
	TextBox(const Font* font, const Vei2& pos, std::string text = "")
		:
		InterfaceObject(font, text, pos)
	{
		SetTextAlignCenter(false);
	}
	void ProcessMouse(const Mouse::Event& event) override
	{
		// hovered
		hovered = GetRect().IsContains(event.GetPos());

		if (hovered && !hoveredAlready) {
			hoverSound.Play();
			hoveredAlready = true;
		}
		else if (!hovered) {
			hoveredAlready = false;
		}

		// clicked
		if (hovered) {
			clicked = (event.GetType() == Mouse::Event::Type::LPress);
			if (clicked) {
				active = true;
				focused = true;
			}
		}
		else if (event.GetType() == Mouse::Event::Type::LPress) {
			active = false;
			focused = false;
		}
	}
	void Interact(char character)
	{
		if (focused)
		{
			if (character == 8 && !text.empty()) 
			{
				text.pop_back();
				if (isSetDynamicSize) {
					SetSizeWidthContentBox(sizeWidth - font->GetWidthChar());
				}
			}
			else if (character != 8) 
			{
				text += character;
				if (isSetDynamicSize) {
					SetSizeWidthContentBox(sizeWidth + font->GetWidthChar());
				}
			}
		}
	}
	bool IsFocused() const
	{
		return focused;
	}
	void SetFocusOff()
	{
		focused = false;
	}
	int GetSize() const
	{
		return (int)text.size();
	}

private:
	bool focused = false;
	static constexpr Color borderColorFocused = Colors::Green;
};