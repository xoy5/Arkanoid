#include "BrickGrid.h"
#include "algorithm"

BrickGrid::BrickGrid(int brickGridWidth, int topOffset, int paddingX, int paddingY, int widthBrick, int heightBrick, int nRowBricks)
	:
	brickGridWidth(brickGridWidth),
	brickGridWHeight(nRowBricks* (heightBrick + paddingY) - paddingY),
	topOffset(topOffset),
	paddingX(paddingX),
	paddingY(paddingY),
	widthBrick(widthBrick),
	heightBrick(heightBrick),
	nRowBricks(nRowBricks),
	// dynamic calculated
	nColBricks((brickGridWidth + paddingX) / (widthBrick + paddingX)),
	gridPos(Vec2{ ((Graphics::ScreenWidth - brickGridWidth) / 2.0f) + (((brickGridWidth + paddingX) % (widthBrick + paddingX)) / 2.0f), (float)topOffset })
	// gridRect(RectF{ gridPos, (float)brickGridWidth, (float)brickGridWHeight })
{
	assert(Graphics::ScreenWidth >= brickGridWidth);
	Vec2 brickPos = gridPos;

	for (int y = 0; y < nRowBricks; y++, brickPos = Vec2{ gridPos.x, brickPos.y + heightBrick + paddingY }) {
		if (y == 4)
		{
			for (int x = 0; x < nColBricks; x++, brickPos += Vec2{ (float)widthBrick + (float)paddingX, 0 }) {
				if (x % 2 == 0) {
					bricks.emplace_back(new UnbreakableBrick(RectF(brickPos, brickPos + Vec2{ (float)widthBrick, (float)heightBrick })));
				}
			}
		}
		else
		{
			for (int x = 0; x < nColBricks; x++, brickPos += Vec2{ (float)widthBrick + (float)paddingX, 0 }) {
				bricks.emplace_back(new BreakableBrick(RectF(brickPos, brickPos + Vec2{ (float)widthBrick, (float)heightBrick }), x + 1, GetColorByHp(x + 1)));
			}
		}
	}

	brickPos = Vec2{ gridPos.x, brickPos.y + paddingX + heightBrick };

}

BrickGrid::~BrickGrid()
{
	for (Brick* b : bricks) {
		delete b;
	}
}

void BrickGrid::Load(const std::string& filename)
{
	bricks.clear();
	std::ifstream file(filename);
	assert(file);

	size_t nBricks;
	file.read(reinterpret_cast<char*>(&nBricks), sizeof(nBricks));

	for (size_t i = 0; i < nBricks; i++) {
		Brick* brick = nullptr;
		Brick::Type type;
		file.read(reinterpret_cast<char*>(&type), sizeof(type));
		switch (type) {
			case Brick::Type::Unbreakable:
				brick = new UnbreakableBrick;
				brick->Load(file);
				break;
			case Brick::Type::Breakable:
				brick = new BreakableBrick;
				brick->Load(file);
				break;
			default:
				assert(false);
		}
		bricks.emplace_back(brick);
	}

	file.close();
}

void BrickGrid::Save(const std::string& filename)
{
	std::ofstream file(filename);
	assert(file);

	size_t nBricks = bricks.size();
	file.write(reinterpret_cast<char*>(&nBricks), sizeof(nBricks));
	for (Brick* brick : bricks){
		brick->Save(file);
	}

	//std::vector<Brick*> bricks;
	//const int brickGridWidth;
	//const int brickGridWHeight;
	//const int topOffset;
	//const int paddingX;
	//const int paddingY;
	//const int widthBrick;
	//const int heightBrick;

	//const int nRowBricks;
	//const int nColBricks; // in BrickGrid() calculating
	//const Vec2 gridPos




	file.close();
}

void BrickGrid::Draw(Graphics& gfx) const
{
	for (Brick* b : bricks) {
		b->Draw(gfx);
	}
}

//bool BrickGrid::DoBallCollision(Ball& ball, Vec2* pHitPos, bool* pDestroyed)
//{
//	bool collisionHappened = false;
//	//if (ball.GetRect().IsOverlappingWith(gridRect)) {
//	float minBrickDistSq;
//	int brickIndex = -1;
//	const Vec2 ballPosCenter = ball.GetPosCenter();
//
//	for (int i = 0; i < bricks.size(); i++)
//	{
//		if (ball.GetRect().IsOverlappingWith(bricks[i]->GetRectF()))
//		{
//			float distance = (ballPosCenter - bricks[i]->GetPosCenter()).GetLengthSq();
//			if (!collisionHappened || distance < minBrickDistSq) {
//				minBrickDistSq = distance;
//				brickIndex = i;
//				collisionHappened = true;
//			}
//		}
//	}
//
//	if (collisionHappened && brickIndex >= 0) {
//		ball.ResetPaddleCooldown();
//
//		const float epsilon = 0.001f;
//		Vec2 ballVelocity = ball.GetVelocity();
//		RectF brickRect = bricks[brickIndex]->GetRectF();
//
//		bool horizontalOverlap = (ballPosCenter.x >= brickRect.left && ballPosCenter.x <= brickRect.right);
//
//		if (fabs(ballVelocity.x) < epsilon) {
//			ball.ReboundY();
//		}
//		else if (std::signbit(ballVelocity.x) == std::signbit(ballPosCenter.x - bricks[brickIndex]->GetPosCenter().x)) {
//			ball.ReboundY();
//		}
//		else if (horizontalOverlap) {
//			ball.ReboundY();
//		}
//		else {
//			ball.ReboundX();
// 		}
//
//		bricks[brickIndex]->Hitted();
//		if (BreakableBrick* brick = dynamic_cast<BreakableBrick*>(bricks[brickIndex])) {
//			if (*pDestroyed = brick->IsDestroyed()) {
//				if (pHitPos) {
//					*pHitPos = bricks[brickIndex]->GetPosCenter();
//				}
//				delete bricks[brickIndex];
//				bricks[brickIndex] = std::move(bricks.back());
//				bricks.pop_back();
//			}
//		}
//	}
//	return collisionHappened;
//}


std::pair<void*, int> BrickGrid::CheckBallCollision(const Ball& ball) const
{
	void* pBrickCollisionHappened = nullptr;
	//if (ball.GetRect().IsOverlappingWith(gridRect)) {
	float minBrickDistSq;
	int brickIndex = -1;
	const Vec2 ballPosCenter = ball.GetPosCenter();

	for (int i = 0; i < bricks.size(); i++)
	{
		if (ball.GetRect().IsOverlappingWith(bricks[i]->GetRectF()))
		{
			float distance = std::fabs((ballPosCenter - bricks[i]->GetPosCenter()).GetLengthSq());
			if (!pBrickCollisionHappened || distance < minBrickDistSq) {
				pBrickCollisionHappened = bricks[i];
				minBrickDistSq = distance;
				brickIndex = i;
			}
		}
	}

	return std::pair<void*, int>(pBrickCollisionHappened, brickIndex);
}

void BrickGrid::ExecuteBallCollision(Ball& ball, int brickIndex, Vec2* pHitPos, bool* pDestroyed)
{
	assert(brickIndex >= 0);

	ball.ResetPaddleCooldown();

	const float epsilon = 0.001f;
	Vec2 ballVelocity = ball.GetVelocity();
	RectF brickRect = bricks[brickIndex]->GetRectF();
	Vec2 ballPosCenter = ball.GetPosCenter();

	bool horizontalOverlap = (ballPosCenter.x >= brickRect.left && ballPosCenter.x <= brickRect.right);

	if (fabs(ballVelocity.x) < epsilon) {
		ball.ReboundY();
		// ball.DoBrickPrecisionMoveY(brickRect);
	}
	else if (std::signbit(ballVelocity.x) == std::signbit(ballPosCenter.x - bricks[brickIndex]->GetPosCenter().x)) {
		ball.ReboundY();
		// ball.DoBrickPrecisionMoveY(brickRect);
	}
	else if (horizontalOverlap) {
		ball.ReboundY();
		// ball.DoBrickPrecisionMoveY(brickRect);
	}
	else {
		ball.ReboundX();
		// ball.DoBrickPrecisionMoveX(brickRect);
	}

	bricks[brickIndex]->Hitted();
	if (BreakableBrick* brick = dynamic_cast<BreakableBrick*>(bricks[brickIndex])) {
		if (*pDestroyed = brick->IsDestroyed()) {
			if (pHitPos) {
				*pHitPos = bricks[brickIndex]->GetPosCenter();
			}
			delete bricks[brickIndex];
			bricks[brickIndex] = std::move(bricks.back());
			bricks.pop_back();
		}
		else
		{
			UpdateBrickColor(brick);
		}
	}
}


void BrickGrid::UpdateBrickColor(BreakableBrick* pBrick)
{
	pBrick->SetColor(GetColorByHp(pBrick->GetHp()));
}

Color BrickGrid::GetColorByHp(int i) const
{
	assert(i > 0);
	return colorsBricks[std::min(i, colorsBricksSize) - 1];
}

