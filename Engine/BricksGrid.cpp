#include "BricksGrid.h"

BricksGrid::BricksGrid(int bricksGridWidth, int topOffset, int paddingX, int paddingY, int widthBrick, int heightBrick, int nRowBricks )
	:
	bricksGridWidth(bricksGridWidth),
	topOffset(topOffset),
	paddingX(paddingX),
	paddingY(paddingY),
	widthBrick(widthBrick),
	heightBrick(heightBrick),
	nRowBricks(nRowBricks), 
	// dynamic calculated
 	nColBricks((bricksGridWidth + paddingX) / (widthBrick + paddingX)),
	gridPos(Vei2{ ((Graphics::ScreenWidth - bricksGridWidth) / 2) + (((bricksGridWidth + paddingX) % (widthBrick + paddingX)) / 2), topOffset })
{
	assert(Graphics::ScreenWidth >= bricksGridWidth);
	Vei2 brickPos = gridPos;

	for (int y = 0; y < nRowBricks; y++, brickPos = Vei2{ gridPos.x, brickPos.y + heightBrick + paddingY}) {
		for (int x = 0; x < nColBricks; x++, brickPos += Vei2{ widthBrick + paddingX, 0 }) {
			bricks.emplace_back(RectI(brickPos, brickPos + Vei2{ widthBrick, heightBrick }), colorsBricks[y % colorsBricksSize], 1);
		}
	}
}

void BricksGrid::Draw(Graphics& gfx) const
{
	for (const auto& b : bricks) {
		b.Draw(gfx);
	}
}