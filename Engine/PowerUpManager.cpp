#include "PowerUpManager.h"
#include "Game.h"

PowerUpManager::PowerUpManager(std::mt19937& rng)
	:
	rng(rng)
{}

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
	if (from1to100(rng) <= 8) {
		powerUps.emplace_back(PowerUp{PowerUp::Type::BiggerPaddle, posBrickCenter - Vec2{powerUpSizeDimension / 2 - 0.5f, powerUpSizeDimension / 2 - 0.5f}, powerUpSizeDimension, powerUpSpeed});
	}
}

void PowerUpManager::DoCollectAndUsePowerUp(Game& game)
{
	for (int i = 0; i < powerUps.size();) {
		if (powerUps[i].GetRect().IsOverlappingWith(game.paddle.GetRect())) {
			switch (powerUps[i].GetType())
			{
				case PowerUp::Type::AddBall:
					break;
				case PowerUp::Type::BiggerPaddle:
					game.paddle.SetWidth(20);
					break;
				case PowerUp::Type::DoubleBalls:
					break;
			}
			powerUps[i] = std::move(powerUps.back());
			powerUps.pop_back();
		}
		else {
			i++;
		}
	}
}

void PowerUpManager::DoWallCollision(const RectF& rect)
{
	for (int i = 0; i < powerUps.size();) {
		if (!powerUps[i].GetRect().IsContainedBy(rect)) {
			powerUps[i] = std::move(powerUps.back());
			powerUps.pop_back();
		}
		else {
			i++;
		}
	}
}
