
#include "Editor.h"
#include "Game.h"
#include "MainWindow.h"
#include <filesystem>

Editor::Editor(Game& game)
	:
	game(game),
	font(&game.fontSm)
{
}

void Editor::Draw(Graphics& gfx) const
{
	if (editing)
	{
		if (IsHandlingMessage()) {
			messageBox.Draw(gfx);
		}
		else {
			buttonEditMode.Draw(gfx);
			buttonLoad.Draw(gfx);
			buttonSave.Draw(gfx);
			textBox.Draw(gfx);
		}
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

	if (IsHandlingMessage() == false) {
		buttonEditMode.ProcessMouse(event);
		buttonLoad.ProcessMouse(event);
		buttonSave.ProcessMouse(event);
		textBox.DoFocusMouse(game.wnd.mouse);

		if (buttonSave.IsClicked()) {
			const BrickGrid::MessageFile msg = game.gf_brickGrid.Save(textBox.GetText());
			switch (msg)
			{
			case BrickGrid::MessageFile::Saved:
				messageBox.SetButtons(MyMessageBox::Buttons::Ok);
				messageBox.SetText("Saved successfully!");
				break;
			case BrickGrid::MessageFile::AlreadyExists:
				messageBox.SetButtons(MyMessageBox::Buttons::YesNo);
				messageBox.SetText("Already exists!\nWanna overwrite?");
				break;
			}
		}
		else if (buttonLoad.IsClicked()) {
			const BrickGrid::MessageFile msg = game.gf_brickGrid.Load(textBox.GetText());
			switch (msg)
			{
			case BrickGrid::MessageFile::Loaded:
				messageBox.SetButtons(MyMessageBox::Buttons::Ok);
				messageBox.SetText("Loaded successfully!");
				break;
			case BrickGrid::MessageFile::NotExists:
				messageBox.SetButtons(MyMessageBox::Buttons::Ok);
				messageBox.SetText("File doesn't exist!");
				break;
			}
		}
		else if (buttonEditMode.IsClicked()) {
			ChangeEditing();
		}
	}

	if (game.gf_brickGrid.GetMessageFile() != BrickGrid::MessageFile::NoMessage)
	{
		MyMessageBox::ValueButton buttonValue;
		if (messageBox.ProcessMouse(event, &buttonValue))
		{
			switch (game.gf_brickGrid.GetMessageFile())
			{
			case BrickGrid::MessageFile::Saved:
				if (buttonValue == MyMessageBox::ValueButton::Ok) game.gf_brickGrid.SetMessageFileNoMessage();
				break;

			case BrickGrid::MessageFile::NotExists:
				if (buttonValue == MyMessageBox::ValueButton::Ok) game.gf_brickGrid.SetMessageFileNoMessage();
				break;


			case BrickGrid::MessageFile::Loaded:
				if (buttonValue == MyMessageBox::ValueButton::Ok) game.gf_brickGrid.SetMessageFileNoMessage();
				break;

			case BrickGrid::MessageFile::AlreadyExists:
				if (buttonValue == MyMessageBox::ValueButton::Yes)
				{
					messageBox.SetButtons(MyMessageBox::Buttons::Ok);
					messageBox.SetText("Saved successfully!");
					game.gf_brickGrid.DeleteBrickGrid(textBox.GetText());
					game.gf_brickGrid.Save(textBox.GetText());

				}
				else game.gf_brickGrid.SetMessageFileNoMessage();
				break;
			}
		}
	}
}

bool Editor::IsHandlingMessage() const
{
	return game.gf_brickGrid.GetMessageFile() != BrickGrid::MessageFile::NoMessage;
}
