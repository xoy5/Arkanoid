#pragma once
#include "Graphics.h"
#include "Font.h"
#include "TextBox.h"
#include "Button.h"
#include "MyMessageBox.h"

class Game;
class Editor
{
public:
	Editor(Game& game, Font* font);
	void Draw(Graphics& gfx) const;
	void ChangeEditing();
	bool IsEditing() const;
	void ProcessInputChar(char character);
	void ProcessMouse(const Mouse::Event& event);

private:
	Game& game;
	const Font* font;
	bool editing = false;
	Button buttonEditMode = Button(font, "Edit Mode", Vei2{ 20, 20 });
	Button buttonLoad = Button(font, "Load", Vei2{ 20, 80 });
	Button buttonSave = Button(font, "Save", Vei2{ 20, 140 });
	TextBox textBox = TextBox(font, Vei2{ 20, 200 });
	MyMessageBox messageBox = MyMessageBox(font);
};