#pragma once

#include "Rect.h"
#include "Mouse.h"
#include "Button.h"
#include "Font.h"

class SelectionMenu
{
public:
	enum class GameState
	{
		MainMenu,
		Solo,
		Duo,
		EditorMode,
		Count,
		Invalid
	};

public:
	SelectionMenu(const Font* font, const Vei2& pos)
	{
		const std::string texts[] = { "Solo", "Duo", "Editor" };
		auto center = pos;
		const int paddingY = 20;
		for (int i = int(GameState::Solo) - 1; i < int(GameState::Count) - 1; i++)
		{
			buttons.emplace_back(MenuButton<GameState>{font, center, GameState(i+1), texts[i]});
			buttons[i].SetPositionCenter(true);
			buttons[i].SetDynamicSize(false);
			const int sizeWidth = int(texts[2].size()) * font->GetWidthChar();
			buttons[i].SetSizeWidthContentBox(sizeWidth);
			center.y += buttons[i].GetHeight() + paddingY;
		}
	}
	// returns GameState::Something when an entry is clicked, otherwise returns GameState::Invalid
	GameState ProcessMouse(const Mouse::Event& e)
	{
		for (auto& b : buttons)
		{
			b.ProcessMouse(e);
			if (b.IsClicked()) {
				return b.GetOption();
				break;
			}
		}
		return GameState::Invalid;
	}
	void Draw(Graphics& gfx) const
	{
		for (const auto& b : buttons)
		{
			b.Draw(gfx);
		}
	}

private:
	std::vector<MenuButton<GameState>> buttons;
};