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
		InterfaceObject::SetTextAlignCenter();
	}
	void ProcessMouse(const Mouse::Event& event) override
	{
		// hovered
		hovered = GetRect().Contains(event.GetPos());

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

			clicked = clickedIn && event.GetType() == Mouse::Event::Type::LRelease;
		}

		// active
		if (clicked)
		{
			if (hovered) {
				InterfaceObject::SetActive(true);
			}
			else {
				InterfaceObject::SetActive(false);
			}
		}
		   
	}
	void Interact(char character)
	{
		if (focused)
		{
			if (character == 8 && !text.empty()) {
				text.pop_back();
			}
			else if (character != 8) {
				text += character;
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
	RectI GetRect() const override
	{
		return RectI{ pos.x, pos.x + (paddingX * 2) + std::max(int(text.size()) * font->GetWidthChar(), font->GetWidthChar()), pos.y, pos.y + GetHeight() };
	}
private:
	bool focused = false;
	static constexpr Color borderColorFocused = Colors::Green;
};