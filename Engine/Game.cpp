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
	gf_brickGrid({Graphics::ScreenWidth})
{}

void Game::Go()
{
	gfx.BeginFrame();	
	ProcessInput();
	if (buttonEditMode.second)
	{
		if (textBox.IsActive())
		{
			textBox.Interact(wnd.kbd);
		}
	}
	else
	{
		float elapsedTime = ft.Mark();
		while (elapsedTime > 0.0f) 
		{
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
	if (!buttonEditMode.second)
	{
		if (wnd.kbd.KeyIsPressed('R') && mapReset == false) {
			gf_brickGrid = BrickGrid{ 800 };
			mapReset = true;
		}
		else {
			if (!wnd.kbd.KeyIsPressed('R')) {
				mapReset = false;
			}
		}

		if (wnd.kbd.KeyIsPressed('M') && powerUpAddBall == false) {
			gf_ballManager.AddBallOnPaddle();	
			powerUpAddBall = true;
		}
		else {
			if (!wnd.kbd.KeyIsPressed('M')) {
				powerUpAddBall = false;
			}
		}

		if (wnd.kbd.KeyIsPressed('K') && powerUpDoubleBall == false) {
			gf_ballManager.DoubleBallsX();
			powerUpDoubleBall = true;
		}
		else {
			if (!wnd.kbd.KeyIsPressed('K')) {
				powerUpDoubleBall = false;
			}
		}

		if (wnd.kbd.KeyIsPressed('L') && powerUpGrowWidth == false) {
			paddle.GrowWidth();
			powerUpGrowWidth = true;
		}
		else {
			if (!wnd.kbd.KeyIsPressed('L')) {
				powerUpGrowWidth = false;
			}
		}
	}

	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		// buttons
		buttonEditMode.first.ProcessMouse(e);
		if (buttonEditMode.first.IsClicked()) {
			buttonEditMode.second = !buttonEditMode.second;

			if (buttonEditMode.second) {
				buttonEditMode.first.SetText("Play Mode");
			}
			else {
				buttonEditMode.first.SetText("Edit Mode");
			}
		}

		if (buttonEditMode.second) {
			buttonSave.ProcessMouse(e);
			buttonLoad.ProcessMouse(e);

			
			if (buttonSave.IsClicked()) {
				gf_brickGrid.Save(textBox.GetText());
			}
			else if (buttonLoad.IsClicked()) {
				gf_brickGrid.Load(textBox.GetText());
			}

			textBox.DoFocusMouse(wnd.mouse);
		}
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

	// Editor mode
	if (buttonEditMode.second) 
	{
		buttonLoad.Draw(gfx);
		buttonSave.Draw(gfx);
		textBox.Draw(gfx);
		if ((bool)gf_brickGrid.GetMyMessage())
		{
			myMessageBox.Draw(gfx);
		}
	}
	 buttonEditMode.first.Draw(gfx);
} 
