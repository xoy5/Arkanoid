/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include <assert.h>
#include <algorithm>

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	background("Files/Sprites/Pipes340x90.bmp", walls, 4),
	gameStats(&fontLg, "Files/Records.txt", rectStats, 3),
	selectionMenu(&font3Xl, Graphics::GetScreenCenter() - Vei2{ 0, 100 }),
	gf_powerUpManager(*this, "Files/Sprites/PowerUpBox.bmp"),
	gf_brickGrid(*this, "Files/BrickGrid/", "Files/Sprites/BricksRGBOP55x20x4.bmp", "Files/Sprites/UnbreakableBrick550x20.bmp", 8),
	gf_ballManager(*this, "Files/Sprites/Ball19x19.bmp", 480.0f),
	gf_editor(*this, &fontSm),
	paddlePlayer1(Paddle::Player::Player1, "Files/Sprites/Paddle80x90x100x20.bmp", Vec2(walls.GetCenter().x, walls.bottom - 25 - (50 / 2)), 600.0f),
	paddlePlayer2(Paddle::Player::Player2, "Files/Sprites/Paddle80x90x100x20.bmp", Vec2(walls.GetCenter().x, walls.top + 25 + (50 / 2)), 600.0f),
	darkenedSurface(walls.GetWidth(), walls.GetHeight())
{
	darkenedSurface.Fill(Colors::Black);
}

void Game::Go()
{
	gfx.BeginFrame();
	ProcessInput();
	const float elapsedTime = ft.Mark();
	if (!gf_editor.IsEditing())
	{
		float time = elapsedTime;
		while (time > 0.0f) {
			const float dt = std::min(precision, time);
			UpdateModel(dt);
			time -= dt;
		}
		numberOfFrames++;
		timeSecond += elapsedTime;
		if (timeSecond >= 1.0f) {
			timeSecond -= 1.0f;
			FPS = numberOfFrames;
			numberOfFrames = 0;
		}
	}

	ComposeFrame();
	gfx.EndFrame();
}

void Game::ProcessInput()
{
	//////////// KEYBOARD //////////////
	// Keys
	while (!wnd.kbd.KeyIsEmpty())
	{
		const Keyboard::Event keyPressed = wnd.kbd.ReadKey();
		if (keyPressed.IsValid() && keyPressed.IsPress())
		{
			if (keyPressed.GetCode() == VK_ESCAPE) wnd.Kill();
			else if (keyPressed.GetCode() == VK_OEM_3) gameState = SelectionMenu::GameState::MainMenu;

			switch (gameState)
			{
			case SelectionMenu::GameState::Solo:
			case SelectionMenu::GameState::Duo:
				if (keyPressed.GetCode() == VK_OEM_5) hacksMode = !hacksMode;
				else if (hacksMode)
				{
					switch (keyPressed.GetCode())
					{
					case 'M':
						gf_ballManager.AddBallOnPaddlePlayer1();
						if (isTwoPlayerMode) {
							gf_ballManager.AddBallOnPaddlePlayer2();
						}
						break;
					case 'K': gf_ballManager.DoubleBallsX(); break;
					case 'L':
						paddlePlayer1.WidthGrow();
						if (isTwoPlayerMode) {
							paddlePlayer2.WidthGrow();
						}
						break;
					}
				}
				break;
			}
		}
	}
	// Characters
	while (!wnd.kbd.CharIsEmpty())
	{
		const char character = wnd.kbd.ReadChar();
		switch (gameState)
		{
		case SelectionMenu::GameState::EditorMode:
			gf_editor.ProcessInputChar(character);
			break;
		}
	}
	////////////////////////////////////

	////////////// MOUSE ///////////////
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		// buttons
		// editor
		switch (gameState)
		{
		case SelectionMenu::GameState::MainMenu:
		{
			SelectionMenu::GameState state = selectionMenu.ProcessMouse(e);
			if (state != SelectionMenu::GameState::Invalid)
			{
				gameState = state;
			}
			break;
		}
		case SelectionMenu::GameState::EditorMode:
			gf_editor.ProcessMouse(e);
			break;
		}
	}
	////////////////////////////////////
}

void Game::UpdateModel(float dt)
{
	switch (gameState)
	{
	case SelectionMenu::GameState::Solo:
	case SelectionMenu::GameState::Duo:
		background.Update(dt);
		gameStats.Update(dt);
		gf_powerUpManager.Update(dt);
		gf_brickGrid.Update(dt);

		paddlePlayer1.Update(dt, wnd.kbd);
		paddlePlayer1.DoWallCollision(walls);
		if (isTwoPlayerMode) {
			paddlePlayer2.Update(dt, wnd.kbd);
			paddlePlayer2.DoWallCollision(walls);
		}

		gf_powerUpManager.DoCollectAndUsePowerUp();
		gf_ballManager.Update(dt, wnd.kbd);
		gf_ballManager.BrickGrid_DoBallCollision();
		gf_ballManager.Paddle_DoBallCollision();
		gf_powerUpManager.DoWallCollision();
		gf_ballManager.DoWallCollision();
		break;

	case SelectionMenu::GameState::EditorMode:
		break;
	}
}

void Game::ComposeFrame()
{
	switch (gameState)
	{
	case SelectionMenu::GameState::MainMenu:
		selectionMenu.Draw(gfx);
		break;

	case SelectionMenu::GameState::Solo:
	case SelectionMenu::GameState::Duo:
		gameStats.Draw(gfx);
		background.Draw(gfx);
		gf_brickGrid.Draw(gfx);

		paddlePlayer1.Draw(gfx);
		if (isTwoPlayerMode) {
			paddlePlayer2.Draw(gfx);
		}

		gf_powerUpManager.Draw(gfx);
		gf_ballManager.Draw(gfx);

		// Shitty stuf developer
		fontXs.DrawText(std::to_string(FPS), Vei2{ int(wallsPlusBorder.right), 0 }, Colors::White, gfx);
		if (hacksMode) {
			fontXs.DrawText("HACKS", Vei2{ Graphics::GetScreenRect().right, 0 } - Vei2{ 5 * fontXs.GetWidthChar(), 0 }, Colors::Red, gfx);
		}

		//gfx.DrawSprite(walls.left, walls.top, darkenedSurface, SpriteEffect::Ghost{ Colors::Magenta });
		break;

	case SelectionMenu::GameState::EditorMode:
		gf_editor.Draw(gfx);
		break;
	}
}
