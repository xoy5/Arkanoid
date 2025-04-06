#pragma once
#include <string>

#include "Graphics.h"
#include "Colors.h"
#include "Font.h"
#include "Button.h"

class MyMessageBox
{
public:
	enum class Buttons
	{
		YesNo,
		Ok
	};
public:
	MyMessageBox(const Font* font)
		:
		font(font)
	{
	}
	void Draw(Graphics& gfx) const
	{
		gfx.DrawRect(Graphics::GetScreenRect().GetExpanded(-200), Colors::Gray);
		switch (buttons)
		{
			case Buttons::YesNo:
				buttonYes.Draw(gfx);
				buttonNo.Draw(gfx);
				break;
			case Buttons::Ok:
				buttonOk.Draw(gfx);
				break;
		}
	}
	void SetType(Buttons buttons)
	{
		this->buttons = buttons;
	}
private:

	Buttons buttons = Buttons::YesNo;
	const Font* font;
	Button buttonYes = Button(font, "Yes");
	Button buttonNo = Button(font, "No");
	Button buttonOk = Button(font, "Ok");

};