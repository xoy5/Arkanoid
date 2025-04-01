#pragma once
#include <vector>
#include <assert.h>
#include <fstream>
#include <string>

#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"

#include "Brick.h"
#include "Attributes.h"
#include "Ball.h"


class BrickGrid
{
public:
	BrickGrid(int brickGridWidth = 600, int topOffset = 15, int paddingX = 4, int paddingY = 4, int widthBrick = 80, int heightBrick = 30, int nRowBricks = 8);
	~BrickGrid();

	static BrickGrid* LoadNewBrickGrid(const std::string& filename = "default.dat");
	//void LoadAtThisBrickGrid(const std::string& filename = "default.dat");
	void SaveBrickGrid(const std::string& filename = "default.dat"); 

	void Draw(Graphics& gfx) const;

	//bool DoBallCollision(Ball& ball, Vec2* pHitPos, bool* pDestroyed);
	std::pair<void*, int> CheckBallCollision(const Ball& ball) const;
	void ExecuteBallCollision(Ball& ball, int BrickIndex, Vec2* pHitPos = nullptr, bool* pDestroyed = nullptr);

private:
	void UpdateBrickColor(BreakableBrick* pBrick);
	Color GetColorByHp(int i) const;

private:
	std::vector<Brick*> bricks;
	const int brickGridWidth;
	const int brickGridWHeight;
	const int topOffset;
	const int paddingX;
	const int paddingY;
	const int widthBrick;
	const int heightBrick;

	const int nRowBricks;
	const int nColBricks; // in BrickGrid() calculating
	const Vec2 gridPos;
	// const RectF gridRect; because I can't add some bricks off the grid

	constexpr static Color colorsBricks[] = { Colors::Red, Colors::Grapefruit, Colors::Pink, Colors::Purple, Colors::Green, Colors::Yellow, Colors::Blue, Colors::Cyan };
	constexpr static int colorsBricksSize = 8;
};
