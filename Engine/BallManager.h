#pragma once
#include <vector>

#include "Graphics.h"
#include "Keyboard.h"

#include "Ball.h"

class Game;
class BallManager
{
public:
	BallManager(Game& game);
	BallManager(Game& game, Vec2 ballPos);
	~BallManager();
	void Draw(Graphics& gfx) const;
	void Update(float dt, Keyboard& kbd);

public:
	void ClearBalls();
	void Paddle_DoBallCollision();
	void BrickGrid_DoBallCollision();
	void DoWallCollision();
	void AddBallOnPaddlePlayer1(); 
	void AddBallOnPaddlePlayer2();
	void DoubleBallsX();

private:
	Game& game;
	std::vector<Ball> balls;
	Ball* pBallOnPaddlePlayer1 = nullptr;
	Ball* pBallOnPaddlePlayer2 = nullptr;
	bool curBallOnPaddle = false;
	static constexpr int nMaxBalls = 100;
};