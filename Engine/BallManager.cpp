#include "BallManager.h"
#include "Game.h"

BallManager::BallManager(Game& game, const std::string& filenameBallSprite, float speed)
	:
	game(game),
	sprite(filenameBallSprite),
	ballsSpeed(speed)
{
	AddBallOnPaddlePlayer1();
	if (game.isTwoPlayerMode) {
		AddBallOnPaddlePlayer2();
	}
}

BallManager::BallManager(Game& game, const std::string& filenameBallSprite, Vec2 ballPos, float speed)
	:
	game(game),
	sprite(filenameBallSprite),
	ballsSpeed(speed)
{
	balls.emplace_back(Ball{&sprite, ballPos, false, ballsSpeed, ballsRadius, Paddle::Player::None});
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

	game.fontXs.DrawText(std::to_string(balls.size()), Vei2(game.wallsPlusBorder.right + 50, 0), Colors::RedOrange, gfx);
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
				if (game.gameState == SelectionMenu::GameState::Solo)
				{
					game.gf_powerUpManager.AddRng(pos, Vec2{ 0.0f, 1.0f });
					game.gameStats.AddPoints(100);
				}
				else
				{
					if (b.GetLastPlayerRebound() == Paddle::Player::Player1) {
						game.gf_powerUpManager.AddRng(pos, Vec2{ 0.0f, 1.0f });
					}
					else if (b.GetLastPlayerRebound() == Paddle::Player::Player2) {
						game.gf_powerUpManager.AddRng(pos, Vec2{ 0.0f, -1.0f });
					}
				}
			};	
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
		float offset = float(game.paddlePlayer1.GetHeight()) / 2.0f + ballsRadius;
		pBallOnPaddlePlayer1 = new Ball(&sprite, game.paddlePlayer1.GetRect().GetCenter() - Vec2{ 0.0f, ballsOffsetOnPaddle }, true, ballsSpeed, ballsRadius, Paddle::Player::Player1 );
	}
}

void BallManager::AddBallOnPaddlePlayer2()
{
	if (pBallOnPaddlePlayer2 == nullptr && int(balls.size()) < nMaxBalls) {
		pBallOnPaddlePlayer2 = new Ball(&sprite, game.paddlePlayer2.GetRect().GetCenter() + Vec2{ 0.0f, ballsOffsetOnPaddle }, true, ballsSpeed, ballsRadius, Paddle::Player::Player2 );
	}
}

void BallManager::DoubleBallsX()
{
	if (balls.size() < nMaxBalls)
	{
		const size_t ballsSize = balls.size();
		for (size_t i = 0; i < std::min(nMaxBalls - ballsSize, ballsSize); i++) {
			Ball ball = balls[i];
			ball.ReboundX();
			balls.emplace_back(ball);
		}
	}
}

const Ball& BallManager::GetFirstBall() const
{
	return balls[0];
}