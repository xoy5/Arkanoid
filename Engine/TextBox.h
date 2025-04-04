#pragma once
#include <string>
#include "Font.h"
#include "Vec2.h"
#include "Mouse.h"
#include "Keyboard.h"
#include <vector>
class TextBox
{
public:
	TextBox(const Font* font, const Vei2& pos)
		:
		font(font),
		pos(pos),
		rect(pos.x, pos.x + width, pos.y, pos.y + height)
	{
	}
	void Draw(Graphics& gfx) const
	{
		gfx.DrawRect(rect, backColor);
		font->DrawText(text, pos, fontColor, gfx);
	}
	void DoFocusMouse(const Mouse& mouse)
	{
		Vei2 mousePos = mouse.GetPos();
		if (mouse.LeftIsPressed())
		{
			isFocused = rect.Contains(mousePos);
		}
	}
    void Interact(Keyboard& kbd)
    {
        if (isFocused)
        {
            if (kbd.KeyIsPressed(VK_ESCAPE))
            {
                isFocused = false;
            }
            else
            {
                bool anyKeyPressed = false;
                if (kbd.KeyIsPressed(VK_BACK))
                {
                    if (!text.empty() && !stillPressed)
                    {
                        text.pop_back();
                    }
					if (!stillPressed)
					{
						stillPressed = true;
					}
					anyKeyPressed = true;
                }
				else
				{
					for (int c = 32; c < 127; c++)
					{
						if (kbd.KeyIsPressed(char(c)))
						{
							if (!stillPressed)
							{
								text += char(c);
								stillPressed = true;
							}
							anyKeyPressed = true;
							break;
						}
					}
				}

                if (!anyKeyPressed)
                {
                    stillPressed = false;
                }
            }
        }
    }
	bool IsActive() const
	{
		return isFocused;
	}
    std::string GetText() const
    {
		return text;
    }
private:
	bool stillPressed = false;
	Vei2 pos;
	int height = 30;
	int width = 100;
	RectI rect;
	Vei2 textCursorPos = { 0,0 };
	bool isFocused = false;
	Color backColor = Colors::White;
	Color fontColor = Colors::Black;
	std::string text;
	const Font* font;
};