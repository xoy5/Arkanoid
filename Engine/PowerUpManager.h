#pragma once
#include <vector>
#include "PowerUp.h"
#include "Graphics.h"
#include "Brick.h"
#include <random>

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
	const int powerUpSizeDimension = 25;
	const float powerUpSpeed = 200.0f;
	std::uniform_int_distribution<int> from1to100 = std::uniform_int_distribution<int>(1, 100);
	std::uniform_int_distribution<int> choosePowerUpType = std::uniform_int_distribution<int>(0, int(PowerUp::Type::Count)-1);
};