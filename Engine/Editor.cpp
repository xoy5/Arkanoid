#include "Editor.h"
#include "Game.h"
#include "MainWindow.h"

Editor::Editor(Game& game, Font* font)
	:
	game(game),
	font(font)
{}

void Editor::Draw(Graphics& gfx) const
{
	if (editing)
	{
		buttonEditMode.Draw(gfx);
		buttonLoad.Draw(gfx);
		buttonSave.Draw(gfx);
		textBox.Draw(gfx);
		messageBox.Draw(gfx);
	}
}

void Editor::ChangeEditing()
{
	textBox.SetText("");
	if (editing == true) {
		textBox.SetFocusOff();
	}
	else {
		game.wnd.kbd.Flush();
	}
	editing = !editing;
	buttonEditMode.SetText(editing ? "Play Mode" : "Edit Mode");
}

bool Editor::IsEditing() const
{
	return editing;
}

void Editor::ProcessInputChar(char character)
{
	if (editing) {
		textBox.Interact(character);
	}
}

void Editor::ProcessMouse(const Mouse::Event& event)
{
	if (!editing) return;
	buttonSave.ProcessMouse(event);
	buttonLoad.ProcessMouse(event);
	buttonEditMode.ProcessMouse(event);
	textBox.DoFocusMouse(game.wnd.mouse);

	if (buttonSave.IsClicked()) {
		game.gf_brickGrid.Save(textBox.GetText());
	}
	else if (buttonLoad.IsClicked()) {
		game.gf_brickGrid.Load(textBox.GetText());
	}
	else if (buttonEditMode.IsClicked()) {
		ChangeEditing();
	}
}
