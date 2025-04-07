#pragma once
#include <vector>
#include <random>

#include "Graphics.h"
#include "Surface.h"

#include "PowerUp.h"
#include "Brick.h"

class Game;
class PowerUpManager
{
public:
	PowerUpManager(Game& game);
	void Draw(Graphics& gfx) const;
	void Update(float dt);
	void AddRng(const Vec2& posBrickCenter);
	void DoCollectAndUsePowerUp();
	void DoWallCollision();
private:
	Game& game;
	std::vector<PowerUp> powerUps;
	const float powerUpSpeed = 200.0f;
	static constexpr int powerUpSizeDimension = 30;
	static constexpr int chanceOfDropPowerUp = 10;
	std::uniform_int_distribution<int> from1to100 = std::uniform_int_distribution<int>(1, 100);
	std::uniform_int_distribution<int> choosePowerUpType = std::uniform_int_distribution<int>(0, int(PowerUp::Type::Count)-1);
};