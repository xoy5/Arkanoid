#pragma once
#include <string>

#include "Graphics.h"
#include "Mouse.h"
#include "Font.h"
#include "Rect.h"
#include "Sound.h"
#include "Vec2.h"
#include "InterfaceObject.h"

typedef InterfaceObject Button;

template<typename O>
class MenuButton : public Button
{
public:
	MenuButton(const Font* font, const Vei2& pos, const O& option, const std::string& text)
		:
		Button(font, text, pos),
		option(option)
	{}
	O GetOption() const 
	{
		return option;
	}
private:
	O option;
};

template<typename T>
class StateButton : public Button
{
private:
	enum class State 
	{
		First,
		Second
	};
public:
	StateButton(const Font* font, const Vei2& pos, T firstStateValue, T secondStateValue,
		const std::string& firstString, const std::string& secondString,
		const Color& firstColor = Colors::LightGreen, const Color& secondColor = Colors::LightCoral)
			:
		Button(font, firstString, pos),
		firstStateValue(firstStateValue),
		secondStateValue(secondStateValue),
		firstString(firstString),
		secondString(secondString),
		firstBackgroundColor(firstColor),
		secondBackgroundColor(secondColor)
	{
		SetBackground(true, firstColor);
		int longestSize = int(std::max(firstString.size(), secondString.size()));
		int sizeWidth = longestSize * font->GetWidthChar();
		SetDynamicSize(false);
		SetSizeWidthContentBox(sizeWidth);
	}

	T GetActiveStateValue() const
	{
		return (bool(state) ? secondStateValue : firstStateValue);
	}


	void ProcessMouse(const Mouse::Event& e) override
	{
		Button::ProcessMouse(e);
		if (IsClicked()) {
			state = State(!bool(state));
			SetText(bool(state) ? secondString : firstString);
			SetBackground(true, bool(state) ?  secondBackgroundColor : firstBackgroundColor);
		}
	}
private:
	State state = State::First;
	T firstStateValue;
	T secondStateValue;
	std::string firstString;
	std::string secondString;
	Color firstBackgroundColor;
	Color secondBackgroundColor;
};