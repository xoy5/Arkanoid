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
	gridPos(Vec2{ ((Graphics::ScreenWidth - bricksGridWidth) / 2.0f) + (((bricksGridWidth + paddingX) % (widthBrick + paddingX)) / 2.0f), (float)topOffset }),
	gridRect(RectF{ gridPos, (float)bricksGridWidth, (float)bricksGridHeight })
{
	assert(Graphics::ScreenWidth >= bricksGridWidth);
	Vec2 brickPos = gridPos;

	for (int y = 0; y < nRowBricks; y++, brickPos = Vec2{ gridPos.x, brickPos.y + heightBrick + paddingY }) {
		for (int x = 0; x < nColBricks; x++, brickPos += Vec2{ (float)widthBrick + (float)paddingX, 0 }) {
			bricks.emplace_back(RectF(brickPos, brickPos + Vec2{ (float)widthBrick, (float)heightBrick }), colorsBricks[y % colorsBricksSize], 1);
		}
	}
}

void BricksGrid::Draw(Graphics& gfx) const
{
	for (const auto& b : bricks) {
		b.Draw(gfx);
	}
}

bool BricksGrid::DoBallCollision(Ball& ball)
{
    bool collisionHappened = false;
    if (ball.GetRectF().IsOverlappingWith(gridRect)) {
        float minBrickDistSq;
        int brickIndex = -1;
        const Vec2 ballPosCenter = ball.GetPosCenter();

        for (int i = 0; i < bricks.size(); i++)
        {
            if (ball.GetRectF().IsOverlappingWith(bricks[i].GetRectF()))
            {
                float distance = (ballPosCenter - bricks[i].GetPosCenter()).GetLengthSq();
                if (!collisionHappened || distance < minBrickDistSq) {
                    minBrickDistSq = distance;
                    brickIndex = i;
                    collisionHappened = true;
                }
            }
        }

        if (collisionHappened && brickIndex >= 0) {
            ball.ResetPaddleCooldown();

            const float epsilon = 0.001f;
            Vec2 ballVelocity = ball.GetVelocity();
            RectF brickRect = bricks[brickIndex].GetRectF();

            bool horizontalOverlap = (ballPosCenter.x >= brickRect.left && ballPosCenter.x <= brickRect.right);

            if (fabs(ballVelocity.x) < epsilon) {
                ball.ReboundY();
                // ball.DoBrickPrecisionMoveY(brickRect);
            }
            else if (std::signbit(ballVelocity.x) == std::signbit(ballPosCenter.x - bricks[brickIndex].GetPosCenter().x)) {
                ball.ReboundY();
                //ball.DoBrickPrecisionMoveY(brickRect);
            }
            else if (horizontalOverlap) {
                ball.ReboundY();
                //ball.DoBrickPrecisionMoveY(brickRect);
            }
            else {
                ball.ReboundX();
                //ball.DoBrickPrecisionMoveX(brickRect);
            }

            bricks[brickIndex].Hitted();
            if (bricks[brickIndex].GetHp() <= 0) {
                bricks[brickIndex] = std::move(bricks.back());
                bricks.pop_back();
            }
        }
    }
    return collisionHappened;
}

