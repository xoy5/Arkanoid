#include "BallManager.h"
#include "Game.h"

BallManager::BallManager(const Vec2& posCenter, float speed, int radius, const Color& color)
{
	balls.emplace_back(Ball{ posCenter, speed, radius, color });
}

BallManager::BallManager(float speed, int radius, const Color& color)
{
	balls.emplace_back(Ball{ speed, radius, color });
}

void BallManager::AddBallOnPaddle()
{

}

void BallManager::Draw(Graphics& gfx) const
{
	for (const auto& b : balls) {
		b.Draw(gfx);
	}
}

void BallManager::Paddle_DoBallCollision(Game& game)
{
	for (auto& b : balls) {
		game.paddle.DoBallCollision(b);
	}
}

void BallManager::BrickGrid_DoBallCollision(Game& game)
{
	for (auto& b : balls) {
		Vec2 pos;
		bool destroyed = false; // if destroyed change to true
		if (game.brickGrid.DoBallCollision(b, &pos, &destroyed)){
			if (destroyed) {
				game.powerUpManager.AddRng(pos);
			}
		}
	}
}

void BallManager::Update(float dt)
{
	for (auto& b : balls) {
		b.Update(dt);
	}
}

void BallManager::DoWallCollision(const RectF& rect)
{
	for (auto& b : balls) {
		b.DoWallCollision(rect);
	}
}

