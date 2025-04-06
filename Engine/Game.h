/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include <vector>
#include <random>

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "FrameTimer.h"
#include "Font.h"
#include "Button.h"
#include "TextBox.h"

#include "Editor.h"
#include "BrickGrid.h"
#include "Paddle.h"
#include "BallManager.h"
#include "PowerUpManager.h"
#include "MyMessageBox.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ProcessInput();
	void UpdateModel(float dt);
	void ComposeFrame();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	std::mt19937 rng = std::mt19937(std::random_device{}());
	FrameTimer ft;
	const float precision = 0.0025f;
	const Font fontSm = Font("Files/Fonts/font16x28.bmp");
	const Font fontLg = Font("Files/Fonts/font32x56.bmp");
	const RectF walls = RectF(Graphics::GetScreenRect());
	MyMessageBox myMessageBox = MyMessageBox(&fontSm);
	/********************************/
	/*  User Variables  */
	// Friends;
	friend class PowerUpManager;
	friend class BallManager;
	friend class BrickGrid;
	friend class Editor;

	// Objects
	Paddle paddle;
	BrickGrid gf_brickGrid;
	BallManager gf_ballManager;
	PowerUpManager gf_powerUpManager;
	Editor gf_editor;

	// TO DO
	// Edit controll state instead BrickGrid
	// key for Hacks
	// FPS
	// Images for power ups
	// Better collision based on circle
	// Animation
	// Sounds
	// Editor Adding Bricks etc
	// Levels
	// Menu
	// 2 Players

	// Web For Game
	// Database with custom levels
	// Dowload Levels and insert 
	// Edit Menu

	/********************************/
};
