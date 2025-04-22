#include "BallManager.h"
#include "Game.h"

BallManager::BallManager(Game& game, float speed, float radius)
	:
	game(game),
	ballsSpeed(speed),
	ballsRadius(radius)
{
	AddBallOnPaddlePlayer1();
	if (game.isTwoPlayerMode) {
		AddBallOnPaddlePlayer2();
	}
}

BallManager::BallManager(Game& game, Vec2 ballPos, float radius, float speed)
	:
	game(game),
	ballsSpeed(speed),
	ballsRadius(radius)
{
	balls.emplace_back(Ball{ballPos, false, ballsSpeed, ballsRadius});
}

BallManager::~BallManager()
{
	delete pBallOnPaddlePlayer1;
	delete pBallOnPaddlePlayer2;
}

void BallManager::Draw(Graphics& gfx) const
{
	if (pBallOnPaddlePlayer1) {
		pBallOnPaddlePlayer1->Draw(gfx);
	}
	if (game.isTwoPlayerMode && pBallOnPaddlePlayer2) {
		pBallOnPaddlePlayer2->Draw(gfx);
	}
	for (const auto& b : balls) {
		b.Draw(gfx);
	}
}

void BallManager::Update(float dt, Keyboard& kbd)
{
	if(pBallOnPaddlePlayer1)
	{
		if (kbd.KeyIsPressed('W')) {
			balls.emplace_back(std::move(*pBallOnPaddlePlayer1));
			delete pBallOnPaddlePlayer1;
			pBallOnPaddlePlayer1 = nullptr;
		}
		else if (pBallOnPaddlePlayer1) {
			pBallOnPaddlePlayer1->UpdateByPaddleX(game.paddlePlayer1.GetRect().GetCenter().x);
		}
	}
	if (game.isTwoPlayerMode && pBallOnPaddlePlayer2)
	{
		if (kbd.KeyIsPressed(VK_UP)) {
			balls.emplace_back(std::move(*pBallOnPaddlePlayer2));
			delete pBallOnPaddlePlayer2;
			pBallOnPaddlePlayer2 = nullptr;
		}
		else if (pBallOnPaddlePlayer2) {
			pBallOnPaddlePlayer2->UpdateByPaddleX(game.paddlePlayer2.GetRect().GetCenter().x);
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
		game.paddlePlayer1.DoBallCollision(b);
		if (game.isTwoPlayerMode) {
			game.paddlePlayer2.DoBallCollision(b);
		}
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
		case Ball::WallHit::TopWallHit:
			if (game.isTwoPlayerMode) {
				balls[i] = std::move(balls.back());
				balls.pop_back();
			}
			else {
				balls[i].SetLastObjectReboundPtr(&game.walls);
				i++;
			}
			break;
		case Ball::WallHit::WallHit:
			balls[i].SetLastObjectReboundPtr(&game.walls);
			i++;
			break;
		case Ball::WallHit::NoWallHit:
			i++;
			break;
		}
	}
}

void BallManager::AddBallOnPaddlePlayer1()
{
	if (pBallOnPaddlePlayer1 == nullptr && int(balls.size()) < nMaxBalls) {
		float offset = float(game.paddlePlayer1.GetHeight()) / 2.0f + 10.0f;
		pBallOnPaddlePlayer1 = new Ball(game.paddlePlayer1.GetRect().GetCenter() - Vec2{ 0.0f, offset }, true, ballsSpeed, ballsRadius );
	}
}

void BallManager::AddBallOnPaddlePlayer2()
{
	if (pBallOnPaddlePlayer2 == nullptr && int(balls.size()) < nMaxBalls) {
		float offset = float(game.paddlePlayer2.GetHeight()) / 2.0f + 10.0f;
		pBallOnPaddlePlayer2 = new Ball(game.paddlePlayer2.GetRect().GetCenter() + Vec2{ 0.0f, offset }, true, ballsSpeed, ballsRadius);
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
