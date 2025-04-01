#pragma once
#include "Ball.h"
#include "Graphics.h"
#include <vector>
#include "Paddle.h"
#include "Keyboard.h"

class Game;
class BallManager
{
public:
	BallManager(Game& game, const Vec2& posCenter, bool onPaddle = false, float speed = 300.0f, int radius = 10, const Color& color = Colors::White);
	BallManager(Game& game, float speed = 300.0f, int radius = 10, const Color& color = Colors::White);
	void Draw(Graphics& gfx) const;
	void Update(float dt, Keyboard& kbd);
public:
	void Paddle_DoBallCollision();
	void BrickGrid_DoBallCollision();
	void DoWallCollision();
	void AddBallOnPaddle();
	void DoubleBallsX();
private:
	Game& game;
	std::vector<Ball> balls;
	bool curBallOnPaddle = false;
	static constexpr int nMaxBalls = 100 ;
};