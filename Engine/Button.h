#pragma once
#include <string>

#include "Graphics.h"
#include "Mouse.h"
#include "Font.h"
#include "Rect.h"
#include "Sound.h"
#include "Vec2.h"

typedef InterfaceObject Button;

template<typename O>
class MenuButton : public Button
{
public:
	MenuButton() = default;
	MenuButton(const Font* font, const Vei2& pos, const O& option, const std::string& text)
		:
		Button(font, text, pos),
		option(option)
	{}
	O GetOption() const {
		return option;
	}
private:
	O option;
};

template<typename T>
class StateButton : public Button
{
public:
	StateButton(const Font* font, const Vei2& pos, T activeState, T inactiveState,
		const std::string& activeString, const std::string& inactiveString,
		const Color& activeColor, const Color& inactiveColor)
		:
		Button(font, activeString, pos),
		activeState(activeState),
		inactiveState(inactiveState),
		activeString(activeString),
		inactiveString(inactiveString),
		activeColor(activeColor),
		inactiveColor(inactiveColor)
	{
		SetBackground(true, activeColor);
	}
	T GetActiveState() const
	{
		return activeState;
	}
	void ProcessMouse(const Mouse::Event& e) override
	{
		Button::ProcessMouse(e);
		if (IsClicked()) {
			std::swap(activeState, inactiveState);
			std::swap(activeString, inactiveString);
			std::swap(activeColor, inactiveColor);

			SetText(activeString);
			SetBackground(true, activeColor);
		}
	}
private:
	T activeState;
	T inactiveState;
	std::string activeString;
	std::string inactiveString;
	Color activeColor;
	Color inactiveColor;
};