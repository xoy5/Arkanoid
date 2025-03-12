#include "BricksGrid.h"
#include "algorithm"

BricksGrid::BricksGrid(int bricksGridWidth, int topOffset, int paddingX, int paddingY, int widthBrick, int heightBrick, int nRowBricks)
	:
	bricksGridWidth(bricksGridWidth),
	bricksGridHeight(nRowBricks* (heightBrick + paddingY) - paddingY),
	topOffset(topOffset),
	paddingX(paddingX),
	paddingY(paddingY),
	widthBrick(widthBrick),
	heightBrick(heightBrick),
	nRowBricks(nRowBricks),
	// dynamic calculated
	nColBricks((bricksGridWidth + paddingX) / (widthBrick + paddingX)),
	gridPos(Vei2{ ((Graphics::ScreenWidth - bricksGridWidth) / 2) + (((bricksGridWidth + paddingX) % (widthBrick + paddingX)) / 2), topOffset }),
	gridRect(RectI{ gridPos, bricksGridWidth, bricksGridHeight })
{
	assert(Graphics::ScreenWidth >= bricksGridWidth);
	Vei2 brickPos = gridPos;

	for (int y = 0; y < nRowBricks; y++, brickPos = Vei2{ gridPos.x, brickPos.y + heightBrick + paddingY }) {
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

bool BricksGrid::IsAndUpdateBallCollision(const Ball& ball)
{
	bool collisionHappened = false;

	if (ball.GetRectF().IsOverlappingWith(RectF(gridRect))) {
		float minBrickDistSq;
		int brickIndex;
		for (int i = 0; i < bricks.size(); i++)
		{
			if (ball.GetRectF().IsOverlappingWith(RectF(bricks[i].GetRectI())))
			{
				const float distance = (ball.GetPosCenter() - Vec2(bricks[i].GetPosCenter())).GetLengthSq();
				if (!collisionHappened) {
					minBrickDistSq = distance;
					brickIndex = i;
					collisionHappened = true;
				}
				else if (minBrickDistSq > distance) {
					minBrickDistSq = distance;
					brickIndex = i;
				}
			}
		}

		if (collisionHappened) {
			bricks[brickIndex].Hitted();
			if (bricks[brickIndex].GetHp() <= 0) {
				bricks[brickIndex] = std::move(bricks.back());
				bricks.pop_back();
			}
		}
	}

	return collisionHappened;
}
