
#include "Editor.h"
#include "Game.h"
#include "MainWindow.h"
#include <filesystem>

Editor::Editor(Game& game)
	:
	game(game),
	font(&game.fontTiny),
	stateButtonEditBrickGrid(font, Vei2{20, 0}, false, true, "Editor YES", "Editor NO"),
	buttonClearBrickGrid(font, "Clear", Vei2{ 20, stateButtonEditBrickGrid.GetRect().GetHeight() + stateButtonEditBrickGrid.GetPos().y}),
	buttonLoad(font, "Load", Vei2{ 20, buttonClearBrickGrid.GetRect().GetHeight() + buttonClearBrickGrid.GetPos().y }),
	buttonSave(font, "Save", Vei2{ 20, buttonLoad.GetRect().GetHeight() + buttonLoad.GetPos().y }),
	stateButtonBrickType(font, Vei2{ 20, buttonSave.GetRect().GetHeight() + buttonSave.GetPos().y }, Brick::Type::Breakable, Brick::Type::Unbreakable, "Unbreakable", "Breakable"),
	textBoxFilename(font, Vei2{ 20, stateButtonBrickType.GetRect().GetHeight() + stateButtonBrickType.GetPos().y }),
	messageBox(font)
{
}

void Editor::Draw(Graphics& gfx) const
{
	if (editing)
	{
		if (IsHandlingMessage()) {
			messageBox.Draw(gfx);
		}
		else
		{
			stateButtonEditBrickGrid.Draw(gfx);
			stateButtonBrickType.Draw(gfx);
			if (editingBrickGrid == false) {
				buttonLoad.Draw(gfx);
				buttonSave.Draw(gfx);
				buttonClearBrickGrid.Draw(gfx);

				font->DrawText("filename:", { 20, textBoxFilename.GetRect().top }, Colors::White, gfx);
				textBoxFilename.Draw(gfx);
			}
			else {
				newBrick->SetRect(BrickGrid::GetRectBrickForRoundPos(game.wnd.mouse.GetPos()));
				newBrick->Draw(gfx);
			}
		}
	}
}

void Editor::ProcessInputChar(char character)
{
	if (editing) {
		textBoxFilename.Interact(character);
	}
}

void Editor::ProcessMouse(const Mouse::Event& event)
{
	if (editing == false) return;

	if (IsHandlingMessage() == false)
	{
		// Processing Mouse in GUI editor brick mode
		stateButtonEditBrickGrid.ProcessMouse(event);
		stateButtonBrickType.ProcessMouse(event);

		// Editing BrickGrid
		if (stateButtonEditBrickGrid.IsClicked()) {
			editingBrickGrid = !editingBrickGrid;
			if (editingBrickGrid) {
				newBrick = CreateBrickWithDataFromButton();
			}
			else {
				delete newBrick;
			}
		}
		else if (stateButtonBrickType.IsClicked()) {
			newBrick = CreateBrickWithDataFromButton();
		}
		else if(editingBrickGrid && event.GetType() == Mouse::Event::Type::LPress){
			game.gf_brickGrid.AddBrickToGrid(newBrick);
			newBrick = CreateBrickWithDataFromButton();
		}

		// Processing Mouse in GUI
		if (editingBrickGrid == false) {
			buttonLoad.ProcessMouse(event);
			buttonSave.ProcessMouse(event);
			buttonClearBrickGrid.ProcessMouse(event);
			textBoxFilename.ProcessMouse(event);
		}

		if (buttonClearBrickGrid.IsClicked()) {
			game.gf_brickGrid.ClearBrickGrid();
		}
		///////////////// SETTING UP A MESSAGEBOX ////////////////////
		if (editingBrickGrid == false)
		{
			if (buttonSave.IsClicked())
			{
				messageFile = game.gf_brickGrid.Save(textBoxFilename.GetText());
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
				messageFile = game.gf_brickGrid.Load(textBoxFilename.GetText());
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
			if (buttonLoad.IsClicked() || buttonSave.IsClicked())
			{
				if (messageFile == BrickGrid::MessageFile::Error) {
					messageBox.SetButtons(MyMessageBox::Buttons::Ok);
					messageBox.SetText("Error!");
				}
			}
		}
		//////////////////////////////////////////////////////////////

	}
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
				if (buttonValue == MyMessageBox::ValueButton::Yes) {
					messageBox.SetButtons(MyMessageBox::Buttons::Ok);
					messageBox.SetText("Saved successfully!");
					game.gf_brickGrid.DeleteBrickGrid(textBoxFilename.GetText());
					messageFile = game.gf_brickGrid.Save(textBoxFilename.GetText());
				}
				else if (buttonValue == MyMessageBox::ValueButton::No) {
					messageFile = BrickGrid::MessageFile::NoMessage;
				}
				break;

			case BrickGrid::MessageFile::Deleted:
				// Nothing
				break;

			case BrickGrid::MessageFile::Error:
				if (buttonValue == MyMessageBox::ValueButton::Ok) messageFile = BrickGrid::MessageFile::NoMessage;
				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////
}

///// Setters and Getters /////

void Editor::ChangeEditing()
{
	textBoxFilename.SetText("");
	if (editing == true) {
		textBoxFilename.SetFocusOff();
	}
	else {
		game.wnd.kbd.Flush();
	}
	editing = !editing;
}

bool Editor::IsEditing() const
{
	return editing;
}

bool Editor::IsEditingBrickGrid() const
{
	return editingBrickGrid;
}

bool Editor::IsHandlingMessage() const
{
	return messageFile != BrickGrid::MessageFile::NoMessage;
}

Brick* Editor::CreateBrickWithDataFromButton() const
{
	RectF rect = BrickGrid::GetRectBrickForRoundPos(game.wnd.mouse.GetPos());
	return game.gf_brickGrid.CreateBrick(stateButtonBrickType.GetActiveStateValue(), rect);
}