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
#include "MyMessageBox.h"

#include "Editor.h"
#include "BrickGrid.h"
#include "Paddle.h"
#include "BallManager.h"
#include "PowerUpManager.h"
#include "Background.h"
#include "GameStats.h"
#include "SelectionMenu.h"

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
	const Font fontTiny = Font("Files/Fonts/font8x14.bmp");
	const Font fontSm = Font("Files/Fonts/font12x21.bmp");
	const Font fontMd = Font("Files/Fonts/font16x28.bmp");
	const Font fontLg = Font("Files/Fonts/font32x56.bmp");
	//RectF walls = RectF(Vec2{ 20.0f, 20.0f }, 605.0f, 580.0f);
	RectF walls = RectF(Vec2{ 24.0f, 24.0f }, 605.0f, 576.0f);
	MyMessageBox myMessageBox = MyMessageBox(&fontLg);
	FrameTimer ft;
	const float precision = 0.025f;
	/////////// FPS //////////
	int FPS = 0;
	float timeFrame = 0.0f;
	float timeSecond = 0.0f;
	int numberOfFrames = 0;
	//////////////////////////
	/********************************/
	/*  User Variables  */
	bool hacksMode = true;
	bool isTwoPlayerMode = false;
	int curLevel = 1;
	Background background;
	GameStats gameStats;
	SelectionMenu selectionMenu;
	SelectionMenu::GameState gameState = SelectionMenu::GameState::MainMenu;
	

	// Friends;
	friend class PowerUpManager;
	friend class BallManager;
	friend class BrickGrid;
	friend class Editor;

	// Objects
	Paddle paddlePlayer1;
	Paddle paddlePlayer2;
	BrickGrid gf_brickGrid;
	BallManager gf_ballManager;
	PowerUpManager gf_powerUpManager;
	Editor gf_editor;

	// TO DO
	// more Bricks types
	// Enemies 
	// More power ups
	// Animation
	// Sounds
	// Editor Adding Bricks etc
	// Levels
	// Menu
	// Wielowatkowac
	// 2 Players

	// Web For Game
	// Database with custom levels
	// Dowload Levels and insert 
	// Edit Menu
	/********************************/
};
