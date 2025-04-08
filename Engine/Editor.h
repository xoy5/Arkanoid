#pragma once
#include "Graphics.h"
#include "Font.h"
#include "TextBox.h"
#include "Button.h"
#include "MyMessageBox.h"
#include "BrickGrid.h"

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

private:
	BrickGrid::MessageFile messageFile = BrickGrid::MessageFile::NoMessage;
	Game& game;
	const Font* font;
	bool editing = false; 
	bool editingBrickGrid = false;

	Button buttonEditMode = Button(font, "Edit Mode", Vei2{ 20, 20 });
	Button buttonLoad = Button(font, "Load", Vei2{ 20, 80 });
	Button buttonSave = Button(font, "Save", Vei2{ 20, 140 });
	Button buttonEditBrickGrid = Button(font, "Edit BrickGrid", Vei2{ 170, 20 });

	TextBox textBoxFilename = TextBox(font, Vei2{ 200, 200 });
	TextBox textBoxOffset = TextBox(font, Vei2{ 200, 260});
	TextBox textBoxGapX = TextBox(font, Vei2{ 200, 320 });
	TextBox textBoxGapY = TextBox(font, Vei2{ 200, 400 });

	MyMessageBox messageBox = MyMessageBox(font);
};