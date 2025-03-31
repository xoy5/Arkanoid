#include "BallManager.h"
#include "Game.h"

BallManager::BallManager(Game& game, const Vec2& posCenter, bool onPaddle, float speed, int radius, const Color& color)
	:
	game(game)
{
	balls.emplace_back(Ball{ posCenter, onPaddle , speed, radius, color });
}

BallManager::BallManager(Game& game, float speed, int radius, const Color& color)
	:
	game(game)
{
	balls.emplace_back(Ball{ speed, radius, color });
}


void BallManager::Draw(Graphics& gfx) const
{
	for (const auto& b : balls) {
		b.Draw(gfx);
	}
}

void BallManager::Update(float dt, Keyboard& kbd)
{
	if (kbd.KeyIsPressed(VK_SPACE)) {
		for (auto& b : balls) {
			b.SetIsStillAddedOnPaddleToFalse();
		}
	}

	for (auto& b : balls) {
		if (b.GetIsStillAddedOnPaddle())
		{
			b.UpdateByPaddleX(game.paddle.GetRect().GetCenter().x);
		}
		else
		{
			b.Update(dt);
		}
	}
}

void BallManager::Paddle_DoBallCollision()
{
	for (auto& b : balls) {
		game.paddle.DoBallCollision(b);
	}
}

void BallManager::BrickGrid_DoBallCollision()
{
	for (auto& b : balls) {
		Vec2 pos;
		bool destroyed = false; // if destroyed change to true
		if (game.brickGrid.DoBallCollision(b, &pos, &destroyed)) {
			if (destroyed) {
				game.gf_powerUpManager.AddRng(pos);
			}
		}
	}
}

void BallManager::DoWallCollision()
{
	for (int i = 0; i < balls.size();) {
		if (balls[i].DoWallCollision(game.walls) == Ball::WallHit::BottomWallHit) {
			balls[i] = std::move(balls.back());
			balls.pop_back();
		}
		else {
			i++;
		}
	}
}

void BallManager::AddBallOnPaddle()
{
	balls.emplace_back(Ball(game.paddle.GetRect().GetCenter() - Vec2{0.0f, game.paddle.GetHeight() / 2.0f + 15.0f}, true, 300.0f, 10.0f, Colors::White));
}

void BallManager::DoubleBallsX()
{
	const int ballsSize = balls.size();
	for (int i = 0; i < ballsSize; i++) {
		Ball ball = balls[i];
		ball.ReboundX();
		balls.emplace_back(ball);
	}
}
