#pragma once
#include "Graphics.h"

#include <string>

#include "Font.h"
#include "TextBox.h"
#include "Button.h"
#include "MyMessageBox.h"
#include "BrickGrid.h"
#include "Brick.h"
#include "Vec2.h"
#include "Mouse.h"
#include "Rect.h"

class Game;
class Editor
{
public:
	Editor(Game& game, const Font* font, const RectI& rect);
	~Editor();
	void Draw(Graphics& gfx) const;

public:
	void ProcessInputChar(char character);
	void ProcessMouse(const Mouse::Event& event);

public:
	void Reset();
	bool IsHandlingMessage() const;
	bool IsPlaying() const;

private:
	Brick* CreateBrickWithDataFromButton() const;

private:
	std::string dirRounds = "Rounds/";
	std::string dirEdits = "Edits/";
	BrickGrid::MessageFile messageFile = BrickGrid::MessageFile::NoMessage;
	bool playing = false;
	Game& game;
	const Font* font;
	RectI rect;
	RectI brickGridRect;

	Button buttonBackToMenu;
	Button buttonReset;
	Button buttonLoad;
	Button buttonSave;
	std::vector<MenuButton<BreakableBrick::Color>> buttonsColor;
	StateButton<Brick::Type> stateButtonBrickType;
	TextBox textBoxFilename;
	Button buttonPlay;

	BreakableBrick::Color curColor = BreakableBrick::Color::Red;
	Brick* newBrick = nullptr;
	bool newBrickInRect = false;

	MyMessageBox messageBox = MyMessageBox(font);
};