#pragma once
#include "Ball.h"
#include "Graphics.h"
#include <vector>
#include "Paddle.h"

class Game;
class BallManager
{
public:
	BallManager(const Vec2& posCenter, float speed = 300.0f, int radius = 10, const Color& color = Colors::White);
	BallManager(float speed = 300.0f, int radius = 10, const Color& color = Colors::White);
	void AddBallOnPaddle();
	void Draw(Graphics& gfx) const;
	void Paddle_DoBallCollision(Game& game);
	void BrickGrid_DoBallCollision(Game& game);
	void Update(float dt);
	void DoWallCollision(const RectF& rect);
	//void BrickGrid
private:
	std::vector<Ball> balls;
};