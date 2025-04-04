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
		std::pair<void*, int> pairBrick = game.gf_brickGrid.CheckBallCollision(b);
		if (pairBrick.first && pairBrick.first != b.GetLastObjectReboundPtr())
		{
			b.SetLastObjectReboundPtr(pairBrick.first);
			Vec2 pos;
			bool destroyed = false; // if destroyed change to true
			game.gf_brickGrid.ExecuteBallCollision(b, pairBrick.second, &pos, &destroyed);
			if (destroyed) {
				game.gf_powerUpManager.AddRng(pos);
			}
		}
	}
}


void BallManager::DoWallCollision()
{
	for (int i = 0; i < balls.size();) {
		Ball::WallHit state = balls[i].DoWallCollision(game.walls);
		switch(state)
		{
			case Ball::WallHit::BottomWallHit:
				balls[i] = std::move(balls.back());
				balls.pop_back();
				break;
			case Ball::WallHit::WallHit:
				balls[i].SetLastObjectReboundPtr(&game.walls);
				[[fallthrough]];
			/*case Ball::WallHit::NoWallHit:
				balls[i].SetLastObjectReboundPtr(nullptr) DO NOT USE IT, IT GONNA FUCK UP ALL GAME
				JUST SKIP THAT CASE, MAYBE I GONNA NEED NoWallHit ONE DAY TF*/
			default:
				i++;
				break;
		}
	}
}

void BallManager::AddBallOnPaddle()
{
	if (balls.size() < nMaxBalls)
	{
		balls.emplace_back(Ball(game.paddle.GetRect().GetCenter() - Vec2{ 0.0f, game.paddle.GetHeight() / 2.0f + 15.0f }, true, 300.0f, 10.0f, Colors::White));
	}
}

void BallManager::DoubleBallsX()
{
	if (balls.size() < nMaxBalls)
	{
		const size_t ballsSize = balls.size();
		for (size_t i = 0; i < ballsSize; i++)
		{
			Ball ball = balls[i];
			ball.ReboundX();
			balls.emplace_back(ball);
		}
	}
}
