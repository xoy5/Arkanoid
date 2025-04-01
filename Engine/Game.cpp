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
	brickGrid(new BrickGrid(800))
{}

void Game::Go()
{
	gfx.BeginFrame();	
	ProcessInput();
	float elapsedTime = ft.Mark();
	while (elapsedTime > 0.0f) {
		const float dt = std::min(precision, elapsedTime);
		UpdateModel(dt);
		elapsedTime -= dt;
	}
	ComposeFrame();
	gfx.EndFrame();
}

void Game::ProcessInput()
{
	if (wnd.kbd.KeyIsPressed('R') && mapReset == false) {
		brickGrid = new BrickGrid(800);
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

	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		// buttons
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
	brickGrid->Draw(gfx);
	gf_powerUpManager.Draw(gfx);
	paddle.Draw(gfx);
	gf_ballManager.Draw(gfx);
}
