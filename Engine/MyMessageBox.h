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
		font(font),
		buttonYes(font, "yes", Graphics::GetScreenCenter() + Vei2{ 0, Graphics::ScreenHeight / 8 }),
		buttonNo(font, "no", Graphics::GetScreenCenter() + Vei2{ 0, Graphics::ScreenHeight / 8 }),
		buttonOk(font, "ok", Graphics::GetScreenCenter() + Vei2{ 0, Graphics::ScreenHeight / 8 })
	{
		buttonYes.SetPositionCenter(true);
		buttonNo.SetPositionCenter(true);
		buttonOk.SetPositionCenter(true);
		buttonNo.SetSizeWidthContentBox(font->GetWidthChar() * 3);
		int paddingBetween = 20;
		buttonNo.SetPos(buttonNo.GetPos() - Vei2{ buttonNo.GetRect().GetWidth() / 2 + paddingBetween / 2, 0 });
		buttonYes.SetPos(buttonYes.GetPos() + Vei2{ buttonYes.GetRect().GetWidth() / 2 + paddingBetween / 2,0 });

	}
	void Draw(Graphics& gfx) const
	{
		// Dark
		gfx.DrawDisabled(Graphics::GetScreenRect());

		// Background
		gfx.DrawRect(rect, Colors::Gray);

		// Text
		Vei2 posText;
		if (font->NumberOfLines(messageText) == 1) {
			posText = Vei2{ Graphics::GetScreenCenter().x, rect.top + rect.GetHeight() / 4 } - Vei2{int(messageText.size()) * font->GetWidthChar() / 2, 0};
		}
		else {
			posText = Vei2{ Graphics::GetScreenCenter().x, rect.top + rect.GetHeight() / 5 } - Vei2{ font->GetLongestLineSize(messageText) * font->GetWidthChar() / 2, 0 };
		}
		font->DrawText(messageText, posText, Colors::White, gfx);

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
	static constexpr RectI rect = RectI{ Graphics::ScreenWidth / 5, Graphics::ScreenWidth / 5 * 4 , Graphics::ScreenHeight / 5, Graphics::ScreenHeight / 5 * 4 };
	const Font* font;
	std::string messageText = "Default";
	Buttons buttons = MyMessageBox::Buttons::YesNo;

	Button buttonYes;
	Button buttonNo;
	Button buttonOk;
};