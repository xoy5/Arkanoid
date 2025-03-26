#pragma once
#include <vector>
#include "PowerUp.h"
#include "Graphics.h"
#include "Brick.h"

class PowerUpManager
{
public:
	PowerUpManager() = default;
	void Draw(Graphics& gfx) const;
	void Update(float dt);
	void Add(const Vec2& posBrickCenter);
	void CheckPaddlePowerUpCollision(const RectF& rect);
	void DoWallCollision(const RectF& rect);
private:
	std::vector<PowerUp> powerUps;
	const int powerUpSizeDimension = 25;
	const float powerUpSpeed = 200.0f;
};