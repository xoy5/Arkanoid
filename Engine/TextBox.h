#pragma once
#include <string>
#include <vector>

#include "Mouse.h"
#include "Vec2.h"
#include "Font.h"

class TextBox
{
public:
	TextBox(const Font* font, const Vei2& pos)
		:
		font(font),
		pos(pos)
	{
	}
	void Draw(Graphics& gfx) const
	{
		if (focused) {
			gfx.DrawRect(GetRect(), borderColor);
			gfx.DrawRect(GetRect().GetExpanded(-3), backColor);
		}
		else {
			gfx.DrawRect(GetRect(), backColor);
		}
		font->DrawText(text, Vei2{ pos.x + paddingX, pos.y + height / 2 - font->GetHeightChar() / 2 }, fontColor, gfx);
	}
	void DoFocusMouse(const Mouse& mouse)
	{
		Vei2 mousePos = mouse.GetPos();
		if (mouse.LeftIsPressed())
		{
			focused = GetRect().Contains(mousePos);
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
	bool IsActive() const
	{
		return focused;
	}
	std::string GetText() const
	{
		return text;
	}
	void SetText(const std::string& text)
	{
		this->text = text;
	}
	void SetFocusOff()
	{
		focused = false;
	}
	RectI GetRect() const
	{
		return RectI{ pos.x, pos.x + (paddingX * 2) + std::max(int(text.size()) * font->GetWidthChar(), font->GetWidthChar()), pos.y, pos.y + height };
	}
	static constexpr Vei2 GetPadding()
	{
		return Vei2{ paddingX, paddingY };
	}

private:
	const Font* font;
	static constexpr int paddingX = 10;
	static constexpr int paddingY = 4;
	int height = font->GetHeightChar() + paddingY * 2;

	std::string text;
	Vei2 pos;
	bool focused = false;

	Color backColor = Colors::White;
	Color borderColor = Colors::Green;
	Color fontColor = Colors::Black;
};