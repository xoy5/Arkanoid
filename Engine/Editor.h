#pragma once
#include "Graphics.h"
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
	Editor(Game& game);
	void Draw(Graphics& gfx) const;
	void ChangeEditing();
	bool IsEditing() const;
	void ProcessInputChar(char character);
	void ProcessMouse(const Mouse::Event& event);
	bool IsHandlingMessage() const;
	bool IsEditingBrickGrid() const;

private:
	Brick* CreateBrickWithDataFromButton() const;

private:
	BrickGrid::MessageFile messageFile = BrickGrid::MessageFile::NoMessage;
	Game& game;
	const Font* font;
	bool editing = false; 
	bool editingBrickGrid = false;
	Brick* newBrick = nullptr;

	Button buttonEditBrickGrid;
	Button buttonClearBrickGrid;
	Button buttonLoad;
	Button buttonSave;
	StateButton<Brick::Type> stateButtonBrickType;
	TextBox textBoxFilename;

	MyMessageBox messageBox = MyMessageBox(font);
};