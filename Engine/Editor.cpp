
#include "Editor.h"
#include "Game.h"
#include "MainWindow.h"
#include <filesystem>

Editor::Editor(Game& game, const Font* font, const RectI& rect)
	:
	game(game),
	font(font),
	rect(rect),
	buttonBackToMenu(font, "Back", Vei2{ rect.left, rect.top }),
	buttonClearBrickGrid(font, "Clear", buttonBackToMenu.GetPos() + Vei2{ 0, buttonBackToMenu.GetRect().GetHeight() }),
	stateButtonBrickType(font, buttonClearBrickGrid.GetPos() + Vei2{ 0, buttonClearBrickGrid.GetRect().GetHeight() }, Brick::Type::Breakable, Brick::Type::Unbreakable, "Brk", "Unbrk"),
	//////////////////
	buttonLoad(font, "Load", Vei2{ rect.left, rect.bottom - 190 }),
	buttonSave(font, "Save", buttonLoad.GetPos() + Vei2{ 0, buttonLoad.GetRect().GetHeight() }),
	textBoxFilename(font, buttonSave.GetPos() + Vei2{ 0 , buttonSave.GetRect().GetHeight() }),
	buttonPlay(font, "Play", textBoxFilename.GetPos() + Vei2{ 0, textBoxFilename.GetRect().GetHeight() }),
	messageBox(&game.fontLg),
	newBrick(CreateBrickWithDataFromButton())
{
	buttonsColor.emplace_back(font, stateButtonBrickType.GetPos() + Vei2{ 0, stateButtonBrickType.GetRect().GetHeight() }, BreakableBrick::Color::Red, "Red");
	buttonsColor[0].SetBackground(true, Colors::Red);

	buttonsColor.emplace_back(font, buttonsColor[0].GetPos() + Vei2{ 0, buttonsColor[0].GetRect().GetHeight() }, BreakableBrick::Color::Green, "Green");
	buttonsColor[1].SetBackground(true, Colors::Green);

	buttonsColor.emplace_back(font, buttonsColor[1].GetPos() + Vei2{ 0, buttonsColor[1].GetRect().GetHeight() }, BreakableBrick::Color::Blue, "Blue");
	buttonsColor[2].SetBackground(true, Colors::Blue);

	buttonsColor.emplace_back(font, buttonsColor[2].GetPos() + Vei2{ 0, buttonsColor[2].GetRect().GetHeight() }, BreakableBrick::Color::Orange, "Orange");
	buttonsColor[3].SetBackground(true, Colors::Orange);

	buttonsColor.emplace_back(font, buttonsColor[3].GetPos() + Vei2{ 0, buttonsColor[3].GetRect().GetHeight() }, BreakableBrick::Color::Pink, "Pink");
	buttonsColor[4].SetBackground(true, Colors::Pink);

	textBoxFilename.SetDynamicSize(false);

	const int width = rect.GetWidth();

	buttonBackToMenu.SetSizeWidthBorderBox(width);
	buttonClearBrickGrid.SetSizeWidthBorderBox(width);
	stateButtonBrickType.SetSizeWidthBorderBox(width);

	for (auto& button : buttonsColor) {
		button.SetDisabled(stateButtonBrickType.GetActiveStateValue() != Brick::Type::Breakable);
		button.SetSizeWidthBorderBox(width);
		button.SetFontColor(Colors::White);
	}

	buttonLoad.SetSizeWidthBorderBox(width);
	buttonSave.SetSizeWidthBorderBox(width);
	textBoxFilename.SetSizeWidthBorderBox(width);
	buttonPlay.SetSizeWidthBorderBox(width);
}

Editor::~Editor()
{
	delete newBrick;
	newBrick = nullptr;
}

void Editor::Draw(Graphics& gfx) const
{
	buttonBackToMenu.Draw(gfx);
	buttonClearBrickGrid.Draw(gfx);
	stateButtonBrickType.Draw(gfx);

	for (const auto& button : buttonsColor) {
		button.Draw(gfx);
	}

	buttonLoad.Draw(gfx);
	buttonSave.Draw(gfx);

	font->DrawText("File:", buttonLoad.GetPos() + Vei2{ 5, -font->GetHeightChar() - 5 }, Colors::White, gfx);
	textBoxFilename.Draw(gfx);

	buttonPlay.Draw(gfx);

	if (newBrick && newBrickInWalls) {
		newBrick->Draw(gfx);
	}

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 11; x++)
		{
			// top bottom
			gfx.DrawLine(
				Vei2{ int(game.walls.left + x * BrickGrid::GetBrickWidth()), int(game.walls.top + y * BrickGrid::GetBrickHeight()) },
				Vei2{ int(game.walls.left + x * BrickGrid::GetBrickWidth()), int(game.walls.top + y * BrickGrid::GetBrickHeight() + BrickGrid::GetBrickHeight()) },
				1, Colors::Cyan);

			// left right
			gfx.DrawLine(
				Vei2{ int(game.walls.left + x * BrickGrid::GetBrickWidth()), int(game.walls.top + y * BrickGrid::GetBrickHeight()) },
				Vei2{ int(game.walls.left + x * BrickGrid::GetBrickWidth() + BrickGrid::GetBrickWidth()), int(game.walls.top + y * BrickGrid::GetBrickHeight()) },
				1, Colors::Cyan);
		}
	}

	if (IsHandlingMessage()) {
		messageBox.Draw(gfx);
	}

}

void Editor::ProcessInputChar(char character)
{
	if (textBoxFilename.GetSize() < 9 || character == VK_BACK) {
		textBoxFilename.Interact(character);
	}
}

void Editor::ProcessMouse(const Mouse::Event& event)
{
	if (IsHandlingMessage() == false)
	{
		if (newBrick)
		{
			newBrickInWalls = game.walls.IsContains(Vec2(event.GetPos()));
			newBrick->SetRect(game.gf_brickGrid.GetRectBrickForRoundPos(event.GetPos()));
		}
		// Processing Mouse in GUI
		buttonBackToMenu.ProcessMouse(event);
		buttonClearBrickGrid.ProcessMouse(event);
		stateButtonBrickType.ProcessMouse(event);
		buttonPlay.ProcessMouse(event);

		if (stateButtonBrickType.GetActiveStateValue() == Brick::Type::Breakable)
		{
			for (auto& button : buttonsColor)
			{
				button.ProcessMouse(event);
				if (button.IsClicked())
				{
					curColor = button.GetOption();
					static_cast<BreakableBrick*>(newBrick)->SetColor(curColor);
				}
			}
		}

		if (buttonBackToMenu.IsClicked())
		{
			game.gameState = SelectionMenu::GameState::MainMenu;
			Reset();
		}
		else if (buttonClearBrickGrid.IsClicked())
		{
			game.gf_brickGrid.ClearBrickGrid();
		}
		else if (stateButtonBrickType.IsClicked())
		{
			for (auto& button : buttonsColor) {
				button.SetDisabled(stateButtonBrickType.GetActiveStateValue() != Brick::Type::Breakable);
			}
			delete newBrick;
			newBrick = CreateBrickWithDataFromButton();
		}
		else if (buttonPlay.IsClicked())
		{
			game.gameState = SelectionMenu::GameState::Solo;
		}
		else if (game.walls.IsContains(Vec2(event.GetPos())))
		{
			if (event.GetType() == Mouse::Event::Type::LPress)
			{
				game.gf_brickGrid.AddBrickToGrid(newBrick);
				newBrick = CreateBrickWithDataFromButton();
			}
			else if (event.GetType() == Mouse::Event::Type::RPress)
			{
				game.gf_brickGrid.RemoveFromGrid(event.GetPos());
			}
		}


		///////////////// SETTING UP A MESSAGEBOX ////////////////////
		buttonLoad.ProcessMouse(event);
		buttonSave.ProcessMouse(event);
		textBoxFilename.ProcessMouse(event);
		if (buttonSave.IsClicked())
		{
			messageFile = game.gf_brickGrid.Save(dirEdits, textBoxFilename.GetText());
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
			messageFile = game.gf_brickGrid.Load(dirEdits, textBoxFilename.GetText());
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
					game.gf_brickGrid.DeleteBrickGrid(dirEdits, textBoxFilename.GetText());
					messageFile = game.gf_brickGrid.Save(dirEdits, textBoxFilename.GetText());
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

void Editor::Reset()
{
	textBoxFilename.SetText("");
	textBoxFilename.SetFocusOff();
	game.wnd.kbd.Flush();
}

bool Editor::IsHandlingMessage() const
{
	return messageFile != BrickGrid::MessageFile::NoMessage;
}

Brick* Editor::CreateBrickWithDataFromButton() const
{
	RectF rect = game.gf_brickGrid.GetRectBrickForRoundPos(game.wnd.mouse.GetPos());
	return game.gf_brickGrid.CreateBrick(stateButtonBrickType.GetActiveStateValue(), rect, curColor);
}