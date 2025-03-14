#pragma once
#include <vector>
#include <assert.h>

#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"

#include "Brick.h"
#include "Attributes.h"
#include "Ball.h"


class BricksGrid
{
public:
	BricksGrid(int bricksGridWidth = 600, int topOffset = 15, int paddingX = 4, int paddingY = 4, int widthBrick = 80, int heightBrick = 30, int nRowBricks = 8);
	void Draw(Graphics& gfx) const;
	RectI IsAndUpdateBallCollision(const Ball& ball);
private:
	std::vector<Brick> bricks;
	const int bricksGridWidth;
	const int bricksGridHeight;
	const int topOffset;
	const int paddingX;
	const int paddingY;
	const int widthBrick;
	const int heightBrick;

	const int nRowBricks;
	const int nColBricks; // in BrickGrid() calculating
	const Vei2 gridPos;
	const RectI gridRect;

	constexpr static Color colorsBricks[] = { Colors::Red, Colors::Grapefruit, Colors::Pink, Colors::Purple, Colors::Green, Colors::Yellow, Colors::Blue, Colors::Cyan };
	constexpr static int colorsBricksSize = 8;
};
