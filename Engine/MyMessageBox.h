#pragma once
#include "Button.h"
#include "Font.h"
#include "Graphics.h"
#include <string>
#include "Colors.h"

class MyMessageBox
{
public:
	enum class Buttons
	{
		YesNo,
		Ok
	};
public:
	MyMessageBox(Font* font)
		:
		font(font)
	{}
	void Draw(Graphics& gfx) const
	{
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
	Font* font;
	Button buttonYes = Button(font, "Yes");
	Button buttonNo = Button(font, "No");
	Button buttonOk = Button(font, "Ok");
};