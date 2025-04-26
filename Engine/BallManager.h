#pragma once
#include <vector>
#include <string>

#include "Graphics.h"
#include "Keyboard.h"
#include "Sprite.h"

#include "Ball.h"
#include "Paddle.h"

class Game;
class BallManager
{
public:
	BallManager(Game& game, const std::string& filenameBallSprite, float speed);
	BallManager(Game& game, const std::string& filenameBallSprite, Vec2 ballPos, float speed);
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
	const Ball& GetFirstBall() const;

private:
	Game& game;
	Sprite sprite;
	std::vector<Ball> balls;
	Ball* pBallOnPaddlePlayer1 = nullptr;
	Ball* pBallOnPaddlePlayer2 = nullptr;
	bool curBallOnPaddle = false;
	static constexpr int nMaxBalls = 1000;
	const float ballsSpeed;
	static constexpr float ballsRadius = 10.0f;
	float ballsOffsetOnPaddle = Paddle::GetHeight() / 2.0f + ballsRadius;
};