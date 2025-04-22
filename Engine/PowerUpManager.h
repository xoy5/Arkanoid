#pragma once
#include <vector>
#include <memory>
#include <random>
#include <string>

#include "Graphics.h"
#include "Surface.h"
#include "PowerUp.h"
#include "Brick.h"

class Game;
class PowerUpManager
{
public:
	PowerUpManager(Game& game, const std::string& filename);
	void Draw(Graphics& gfx) const;
	void Update(float dt);

public:
	void AddRng(const Vec2& posBrickCenter);
	void DoCollectAndUsePowerUp();
	void DoWallCollision();

private:
	Game& game;
	std::vector<PowerUp> powerUps;
	const float powerUpSpeed = 300.0f;
	static constexpr int powerUpSizeDimension = 30;
	std::shared_ptr<Surface> spriteBox;
	static constexpr int chanceOfDropPowerUp = 12;
	std::uniform_int_distribution<int> from1to100 = std::uniform_int_distribution<int>(1, 100);
	std::uniform_int_distribution<int> choosePowerUpType = std::uniform_int_distribution<int>(0, int(PowerUp::Type::Count) - 1);
};