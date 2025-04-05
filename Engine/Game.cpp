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
	gf_powerUpManager(*this),
	gf_ballManager(*this, paddle.GetRect().GetCenter() - Vec2{ 0.0f, float(paddle.GetHeight()) / 2.0f + 15.0f }, true),
	gf_brickGrid({ Graphics::ScreenWidth }),
	gf_editor(*this, &fontSm)
{
	wnd.kbd.DisableAutorepeat();
}

void Game::Go()
{
	gfx.BeginFrame();
	float elapsedTime = ft.Mark();
	ProcessInput();
	if (!gf_editor.IsEditing())
	{
		while (elapsedTime > 0.0f) {
			const float dt = std::min(precision, elapsedTime);
			UpdateModel(dt);
			elapsedTime -= dt;
		}
	}
	ComposeFrame();
	gfx.EndFrame();
}

void Game::ProcessInput()
{
	// KEYBOARD
	while (!wnd.kbd.KeyIsEmpty())
	{
		const Keyboard::Event keyPressed = wnd.kbd.ReadKey();
		if (keyPressed.IsValid() && keyPressed.IsPress())
		{
			if (keyPressed.GetCode() == VK_ESCAPE)
				wnd.Kill();
			else if (keyPressed.GetCode() == VK_OEM_3)
				gf_editor.ChangeEditing();
			else if (gf_editor.IsEditing())
			{
				if (keyPressed.GetCode() == 'R') gf_brickGrid.Load();
				if (keyPressed.GetCode() == 'M') gf_ballManager.AddBallOnPaddle();
				if (keyPressed.GetCode() == 'K') gf_ballManager.DoubleBallsX();
				if (keyPressed.GetCode() == 'L') paddle.GrowWidth();
			}
			gf_editor.ProcessInputKeyboard(wnd.kbd);
		}
	}
	// MOUSE
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		// buttons
		gf_editor.ProcessMouse(e);
	}
}

void Game::UpdateModel(float dt)
{
	paddle.Update(dt, wnd.kbd);
	gf_ballManager.Update(dt, wnd.kbd);
	gf_powerUpManager.Update(dt);

	paddle.DoWallCollision(walls);
	gf_ballManager.DoWallCollision();
	gf_powerUpManager.DoWallCollision();

	gf_ballManager.BrickGrid_DoBallCollision();
	gf_ballManager.Paddle_DoBallCollision();
	gf_powerUpManager.DoCollectAndUsePowerUp();
}

void Game::ComposeFrame()
{
	gf_brickGrid.Draw(gfx);
	gf_powerUpManager.Draw(gfx);
	paddle.Draw(gfx);
	gf_ballManager.Draw(gfx);
	gf_editor.Draw(gfx);
}
