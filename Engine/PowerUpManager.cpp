#include "PowerUpManager.h"

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

void PowerUpManager::Add(const Vec2& posBrickCenter)
{
	powerUps.emplace_back(PowerUp{ posBrickCenter - Vec2{powerUpSizeDimension / 2 - 0.5f, powerUpSizeDimension / 2 - 0.5f}, powerUpSizeDimension, powerUpSpeed});
}

void PowerUpManager::CheckPaddlePowerUpCollision(const RectF& rect)
{
	for (int i = 0; i < powerUps.size();) {
		if (powerUps[i].GetRect().IsOverlappingWith(rect)) {
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
