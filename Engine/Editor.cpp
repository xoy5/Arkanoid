
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

	if (IsHandlingMessage() == false) 
	{
		buttonEditMode.ProcessMouse(event);
		buttonLoad.ProcessMouse(event);
		buttonSave.ProcessMouse(event);
		textBox.DoFocusMouse(game.wnd.mouse);

		///////////////// SETTING UP A MESSAGEBOX ////////////////////
		if (buttonSave.IsClicked()) 
		{
			messageFile = game.gf_brickGrid.Save(textBox.GetText());
			switch (messageFile)
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
		else if (buttonLoad.IsClicked()) 
		{
			messageFile = game.gf_brickGrid.Load(textBox.GetText());
			switch (messageFile)
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

		if (buttonLoad.IsClicked() || buttonSave.IsClicked()) 
		{
			if (messageFile == BrickGrid::MessageFile::Error) {
				messageBox.SetButtons(MyMessageBox::Buttons::Ok);
				messageBox.SetText("Error!");
			}
		}
	}
	//////////////////////////////////////////////////////////////
	
	////////////////////  PROCESS MESSAGEBOX /////////////////////
	if (messageFile != BrickGrid::MessageFile::NoMessage)
	{
		MyMessageBox::ValueButton buttonValue;
		if (messageBox.ProcessMouse(event, &buttonValue))
		{
			switch (messageFile)
			{
			case BrickGrid::MessageFile::Saved:
				if (buttonValue == MyMessageBox::ValueButton::Ok) messageFile = BrickGrid::MessageFile::NoMessage;
				break;

			case BrickGrid::MessageFile::NotExists:
				if (buttonValue == MyMessageBox::ValueButton::Ok) messageFile = BrickGrid::MessageFile::NoMessage;
				break;


			case BrickGrid::MessageFile::Loaded:
				if (buttonValue == MyMessageBox::ValueButton::Ok) messageFile = BrickGrid::MessageFile::NoMessage;
				break;

			case BrickGrid::MessageFile::AlreadyExists:
				if (buttonValue == MyMessageBox::ValueButton::Yes){
					messageBox.SetButtons(MyMessageBox::Buttons::Ok);
					messageBox.SetText("Saved successfully!");
					game.gf_brickGrid.DeleteBrickGrid(textBox.GetText());
					messageFile = game.gf_brickGrid.Save(textBox.GetText());
				}
				break;

			case BrickGrid::MessageFile::Deleted:
				// Nothing
				break;

			case BrickGrid::MessageFile::Error:
				if(buttonValue == MyMessageBox::ValueButton::Ok) messageFile = BrickGrid::MessageFile::NoMessage;
				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////
}

bool Editor::IsHandlingMessage() const
{
	return messageFile != BrickGrid::MessageFile::NoMessage;
}
