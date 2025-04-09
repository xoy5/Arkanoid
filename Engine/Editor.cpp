
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
		else
		{
			buttonEditBrickGrid.Draw(gfx);
			if (editingBrickGrid == false) {
				buttonLoad.Draw(gfx);
				buttonSave.Draw(gfx);
				buttonClearBrickGrid.Draw(gfx);

				font->DrawText("filename:", { 20, textBoxFilename.GetRect().top + textBoxFilename.GetPadding().y }, Colors::White, gfx);
				textBoxFilename.Draw(gfx);

				font->DrawText("offset:", { 20, textBoxOffset.GetRect().top + textBoxOffset.GetPadding().y }, Colors::White, gfx);
				textBoxOffset.Draw(gfx);

				font->DrawText("gap x:", { 20, textBoxGapX.GetRect().top + textBoxGapX.GetPadding().y }, Colors::White, gfx);
				textBoxGapX.Draw(gfx);

				font->DrawText("gap y:", { 20, textBoxGapY.GetRect().top + textBoxGapY.GetPadding().y }, Colors::White, gfx);
				textBoxGapY.Draw(gfx);
			}
			else {
				Vec2 brickPos = Vec2(game.wnd.mouse.GetPos());
				const int gapX = std::stoi(textBoxGapX.GetText() == "" ? "0" : textBoxGapX.GetText());
				const int gapY = std::stoi(textBoxGapY.GetText() == "" ? "0" : textBoxGapY.GetText());
				const int fullX = int(brickPos.x) / BrickGrid::GetBrickWidth();
				const int fullY = int(brickPos.x);
				const float restX = (gapX != 0 ? int(brickPos.x) % gapX : 0);
				const float restY = (gapY != 0 ? int(brickPos.y) % gapY : 0);

				if (std::round(float(restX) / float(gapX))) {
					brickPos += Vec2{ gapX - restX, gapY - restY };
				}
				else {
					brickPos -= Vec2{ restX, restY };
				}

				if (newBrick != nullptr) {
					newBrick->SetPos(brickPos);
					//newBrick->Draw(gfx);
				}
			}
		}
	}
}

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

void Editor::ProcessInputChar(char character)
{
	if (editing) {
		textBoxFilename.Interact(character);
		textBoxOffset.Interact(character);
		textBoxGapX.Interact(character);
		textBoxGapY.Interact(character);
	}
}

void Editor::ProcessMouse(const Mouse::Event& event)
{
	if (editing == false) return;

	if (IsHandlingMessage() == false)
	{
		buttonEditBrickGrid.ProcessMouse(event);
		// Editing BrickGrid
		if (buttonEditBrickGrid.IsClicked()) {
			editingBrickGrid = !editingBrickGrid;
			if (editingBrickGrid) newBrick = static_cast<BreakableBrick*>(game.gf_brickGrid.CreateBrick(Brick::Type::Breakable));
			else delete newBrick;
		}
		else if(editingBrickGrid && game.wnd.mouse.LeftIsPressed()){
			game.gf_brickGrid.AddBrickToGrid(newBrick);
			newBrick = static_cast<BreakableBrick*>(game.gf_brickGrid.CreateBrick(Brick::Type::Breakable));
		}

		// Processing Mouse in GUI
		if (editingBrickGrid == false) {
			buttonLoad.ProcessMouse(event);
			buttonSave.ProcessMouse(event);
			buttonClearBrickGrid.ProcessMouse(event);
			textBoxFilename.DoFocusMouse(event);
			textBoxOffset.DoFocusMouse(event);
			textBoxGapX.DoFocusMouse(event);
			textBoxGapY.DoFocusMouse(event);
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

bool Editor::IsHandlingMessage() const
{
	return messageFile != BrickGrid::MessageFile::NoMessage;
}

bool Editor::IsEditingBrickGrid() const
{
	return editingBrickGrid;
}
