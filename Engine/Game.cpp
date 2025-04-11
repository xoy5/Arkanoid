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
#include <thread>

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	gf_powerUpManager(*this),
	gf_ballManager(*this, paddle.GetRect().GetCenter() - Vec2{ 0.0f, float(paddle.GetHeight()) / 2.0f + 15.0f }, true),
	gf_brickGrid(*this, 800, 5, 5, 5),
	gf_editor(*this)
{
	wnd.kbd.DisableAutorepeat();
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
			else if (keyPressed.GetCode() == VK_OEM_3) gf_editor.ChangeEditing();
			else if (keyPressed.GetCode() == VK_OEM_5) hacksMode = !hacksMode;
			else if (hacksMode && !gf_editor.IsEditing())
			{
				switch (keyPressed.GetCode())
				{
				case 'M': gf_ballManager.AddBallOnPaddle(); break;
				case 'K': gf_ballManager.DoubleBallsX(); break;
				case 'L': paddle.GrowWidth(); break;
				}
			}
		}
	}
	// Characters
	while (!wnd.kbd.CharIsEmpty())
	{
		const char character = wnd.kbd.ReadChar();
		gf_editor.ProcessInputChar(character);
	}
	////////////////////////////////////

	////////////// MOUSE ///////////////
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		// buttons
		// editor
		gf_editor.ProcessMouse(e);
	}
	////////////////////////////////////
}

void Game::UpdateModel(float dt)
{
	if (gf_editor.IsHandlingMessage() == false) {
		gf_powerUpManager.Update(dt);
		paddle.Update(dt, wnd.kbd);
		gf_ballManager.Update(dt, wnd.kbd   );


		gf_powerUpManager.DoCollectAndUsePowerUp();

		gf_ballManager.BrickGrid_DoBallCollision();
		gf_ballManager.Paddle_DoBallCollision()
			;
		gf_powerUpManager.DoWallCollision();
		paddle.DoWallCollision(walls);
		gf_ballManager.DoWallCollision();
	}
}

void Game::ComposeFrame()
{
	if (gf_editor.IsHandlingMessage() == false) {
		gf_brickGrid.Draw(gfx);
		paddle.Draw(gfx);
		gf_powerUpManager.Draw(gfx);
		gf_ballManager.Draw(gfx);
		if (gf_editor.IsEditing() == false) {
			fontSm.DrawText(std::to_string(FPS), { 0,0 }, Colors::White, gfx);
			if (hacksMode) {
				fontSm.DrawText("HACKS", Vei2{ Graphics::GetScreenRect().right, 0 } - Vei2{ 5 * fontSm.GetWidthChar(), 0 }, Colors::Red, gfx);
			}
		}
	}
	gf_editor.Draw(gfx);
}
