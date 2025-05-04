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

private:
	Brick* CreateBrickWithDataFromButton() const;

private:
	std::string dirRounds = "Rounds/";
	std::string dirEdits = "Edits/";
	BrickGrid::MessageFile messageFile = BrickGrid::MessageFile::NoMessage;
	Game& game;
	const Font* font;
	RectI rect;

	Button buttonBackToMenu;
	Button buttonClearBrickGrid;
	Button buttonLoad;
	Button buttonSave;
	std::vector<MenuButton<BreakableBrick::Color>> buttonsColor;
	StateButton<Brick::Type> stateButtonBrickType;
	TextBox textBoxFilename;
	Button buttonPlay;

	BreakableBrick::Color curColor = BreakableBrick::Color::Red;
	Brick* newBrick = nullptr;
	bool newBrickInWalls = false;

	MyMessageBox messageBox = MyMessageBox(font);
};