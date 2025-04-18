#include "BallManager.h"
#include "Game.h"

BallManager::BallManager(Game& game)
	:
	game(game)
{
	AddBallOnPaddle();
}

BallManager::BallManager(Game& game, Vec2 ballPos)
	:
	game(game)
{
	balls.emplace_back(Ball{ballPos, false});
}

BallManager::~BallManager()
{
	delete pBallOnPaddle;
}

void BallManager::Draw(Graphics& gfx) const
{
	if (pBallOnPaddle) {
		pBallOnPaddle->Draw(gfx);
	}
	for (const auto& b : balls) {
		b.Draw(gfx);
	}
}

void BallManager::Update(float dt, Keyboard& kbd)
{
	if(pBallOnPaddle)
	{
		if (kbd.KeyIsPressed(VK_SPACE)){
			balls.emplace_back(std::move(*pBallOnPaddle));
			delete pBallOnPaddle;
			pBallOnPaddle = nullptr;
		}
		else if (pBallOnPaddle) {
			pBallOnPaddle->UpdateByPaddleX(game.paddle.GetRect().GetCenter().x);
		}
	}

	for (auto& b : balls){
		b.Update(dt);
	}
}

void BallManager::ClearBalls()
{
	balls.clear();
}

void BallManager::Paddle_DoBallCollision()
{
	for (auto& b : balls) {
		game.paddle.DoBallCollision(b);
	}
}

void BallManager::BrickGrid_DoBallCollision()
{
	for (auto& b : balls)
	{
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
	for (int i = 0; i < balls.size();)
	{
		Ball::WallHit state = balls[i].DoWallCollision(game.walls);
		switch (state)
		{
		case Ball::WallHit::BottomWallHit:
			balls[i] = std::move(balls.back());
			balls.pop_back();
			break;
		case Ball::WallHit::WallHit:
			balls[i].SetLastObjectReboundPtr(&game.walls);
			[[fallthrough]];
		default:
			i++;
			break;
		}
	}
}

void BallManager::AddBallOnPaddle()
{
	if (pBallOnPaddle == nullptr && int(balls.size()) < nMaxBalls) {
		float offset = float(game.paddle.GetHeight()) / 2.0f + 10.0f;
		pBallOnPaddle = new Ball(game.paddle.GetRect().GetCenter() - Vec2{ 0.0f, offset }, true);
	}
}

void BallManager::DoubleBallsX()
{
	if (balls.size() < nMaxBalls)
	{
		const size_t ballsSize = balls.size();
		for (size_t i = 0; i < ballsSize; i++) {
			Ball ball = balls[i];
			ball.ReboundX();
			balls.emplace_back(ball);
		}
	}
}
