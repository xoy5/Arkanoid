#pragma once
#include <string>

#include "Graphics.h"
#include "Mouse.h"
#include "Colors.h"
#include "Font.h"
#include "Button.h"
#include "BrickGrid.h"

class MyMessageBox
{
public:
	enum class Buttons
	{
		YesNo,
		Ok
	};
	enum class ValueButton
	{
		Yes,
		No,
		Ok
	};

public:
	MyMessageBox(const Font* font)
		:
		font(font)
	{
		buttonYes.SetPositionCenter(true);
		buttonYes.PositionMoveBy(Vei2{-40, 40});
		buttonNo.SetPositionCenter(true);
		buttonNo.PositionMoveBy(Vei2{40, 40});
		buttonOk.SetPositionCenter(true);
		buttonOk.PositionMoveBy(Vei2{0, 40});
	}
	void Draw(Graphics& gfx) const
	{
		// Background
		gfx.DrawRect(rect, Colors::Gray);

		// Text
		{
			Vei2 posText;
			if (font->NumberOfLines(messageText) == 1) {
				posText = Vei2{ Graphics::GetScreenCenter().x, rect.top + 65 } - Vei2{ int(messageText.size()) * font->GetWidthChar() / 2, 0 };
			}
			else {
				posText = Vei2{ Graphics::GetScreenCenter().x, rect.top + 45 } - Vei2{ font->GetLongestLineSize(messageText) * font->GetWidthChar() / 2, 0 };
			}
			font->DrawText(messageText, posText, Colors::White, gfx);
		}

		// Buttons
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
	bool ProcessMouse(const Mouse::Event& e, ValueButton* button)
	{
		bool clicked = false;
		if (buttons == Buttons::YesNo) {
			buttonYes.ProcessMouse(e);
			buttonNo.ProcessMouse(e);

			if (buttonYes.IsClicked()) {
				*button = ValueButton::Yes;
				clicked = true;
			}
			else if (buttonNo.IsClicked()) {
				*button = ValueButton::No;
				clicked = true;
			}
		}
		else {
			buttonOk.ProcessMouse(e);
			if (buttonOk.IsClicked()) {
				*button = ValueButton::Ok;
				clicked = true;
			}
		}

		return clicked;
	}
	void SetButtons(Buttons buttons)
	{
		this->buttons = buttons;
	}
	void SetText(const std::string& text)
	{
		messageText = text;
	}
private:
	static constexpr RectI rect = RectI{ 200, 600, 200, 400 };
	std::string messageText = "Default";
	Buttons buttons = MyMessageBox::Buttons::YesNo;
	const Font* font;
	Button buttonYes = Button(font, "Yes");
	Button buttonNo = Button(font, "No");
	Button buttonOk = Button(font, "Ok");
};