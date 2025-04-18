#include "PowerUpManager.h"
#include "Game.h"

PowerUpManager::PowerUpManager(Game& game)
	:
	game(game),
	spriteBox(std::make_shared<Surface>("Files/Sprites/PowerUpBox15x15.bmp"))
{
}

void PowerUpManager::Draw(Graphics& gfx) const
{
	for (const auto& p : powerUps) {
		p.Draw(gfx);
	}
}

void PowerUpManager::Update(float dt)
{
	for (auto& p : powerUps) {
		p.Update(dt);
	}
}

void PowerUpManager::AddRng(const Vec2& posBrickCenter)
{
	if (from1to100(game.rng) <= chanceOfDropPowerUp) {
		powerUps.emplace_back(
			spriteBox,
			PowerUp::Type(choosePowerUpType(game.rng)),
			posBrickCenter - Vec2{ powerUpSizeDimension / 2 - 0.5f, powerUpSizeDimension / 2 - 0.5f },
			powerUpSizeDimension,
			powerUpSpeed
		);
	}
}

void PowerUpManager::DoCollectAndUsePowerUp()
{
	for (int i = 0; i < powerUps.size();) {
		if (powerUps[i].GetRect().IsOverlappingWith(game.paddle.GetRect())) {
			switch (powerUps[i].GetType()) {
			case PowerUp::Type::AddBall:
				game.gf_ballManager.AddBallOnPaddle();
				break;
			case PowerUp::Type::GrowWidthPaddle:
				game.paddle.GrowWidth();
				break;
			case PowerUp::Type::DoubleBalls:
				game.gf_ballManager.DoubleBallsX();
				break;
			}
			powerUps.erase(powerUps.begin() + i);
		}
		else {
			i++;
		}
	}
}

void PowerUpManager::DoWallCollision()
{
	for (int i = 0; i < powerUps.size();) {
		if (!powerUps[i].GetRect().IsContainedBy(game.walls)) {
			powerUps.erase(powerUps.begin() + i);
		}
		else {
			i++;
		}
	}
}
