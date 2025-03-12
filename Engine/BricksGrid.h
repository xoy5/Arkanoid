#pragma once
#include <vector>
#include <assert.h>

#include "Graphics.h"
#include "Brick.h"
#include "Vec2.h"

class BricksGrid
{
public:
	BricksGrid(int bricksGridWidth = 600, int topOffset = 15, int paddingX = 4, int paddingY = 4, int widthBrick = 80, int heightBrick = 30, int nRowBricks = 8);
	void Draw(Graphics& gfx) const;
private:
	std::vector<Brick> bricks;
	const int bricksGridWidth;
	const int topOffset;
	const int paddingX;
	const int paddingY;
	const int widthBrick;
	const int heightBrick;

	const int nRowBricks;
	const int nColBricks; // in BrickGrid() calculating
	const Vei2 gridPos;

	constexpr static Color colorsBricks[] = { Colors::Red, Colors::Grapefruit, Colors::Pink, Colors::Purple, Colors::Green, Colors::Yellow, Colors::Blue, Colors::Cyan };
	constexpr static int colorsBricksSize = 8;
};
