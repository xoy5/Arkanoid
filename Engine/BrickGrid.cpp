#include "BrickGrid.h"
#include "Game.h"
#include <algorithm>
#include <filesystem>

BrickGrid::BrickGrid(Game& game, int brickGridWidth, int topOffset, int paddingX, int paddingY, int widthBrick, int heightBrick, int nRowBricks)
	:
	game(game)
{
	int brickGridWHeight = nRowBricks * (heightBrick + paddingY) - paddingY;
	int nColBricks = (brickGridWidth + paddingX) / (widthBrick + paddingX);
	Vec2 gridPos{ ((Graphics::ScreenWidth - brickGridWidth) / 2.0f) + (((brickGridWidth + paddingX) % (widthBrick + paddingX)) / 2.0f), (float)topOffset };

	assert(Graphics::ScreenWidth >= brickGridWidth);
	Vec2 brickPos = gridPos;

	for (int y = 0; y < nRowBricks; y++, brickPos = Vec2{ gridPos.x, brickPos.y + heightBrick + paddingY }) {
		if (y == 4)
		{
			for (int x = 0; x < nColBricks; x++, brickPos += Vec2{ (float)widthBrick + (float)paddingX, 0 }) 
			{
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

BrickGrid::MessageFile BrickGrid::Load(std::string filename)
{
	MessageFile messageFile = MessageFile::Loaded;

	PrepareFilename(filename);

	std::ifstream file(filename);

	if (!file){
		messageFile = MessageFile::NotExists;
		return messageFile;
	};

	bricks.clear();
	game.gf_ballManager.ClearBalls();
	game.gf_ballManager.AddBallOnPaddle();

	size_t nBricks = 0;
	file.read(reinterpret_cast<char*>(&nBricks), sizeof(nBricks));

	for (size_t i = 0; i < nBricks; i++) 
	{
		Brick* brick = nullptr;
		Brick::Type type;
		file.read(reinterpret_cast<char*>(&type), sizeof(type));
		switch (type)
		{
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
	return messageFile;
}

BrickGrid::MessageFile BrickGrid::Save(std::string filename)
{
	MessageFile  messageFile = MessageFile::Saved;
	PrepareFilename(filename);

	if (std::filesystem::exists(filename)) {
		messageFile = MessageFile::AlreadyExists;
		return messageFile;
	}

	std::ofstream file(filename);

	if (!file.good()){
		messageFile = MessageFile::Error;
		return messageFile;
	}

	size_t nBricks = bricks.size();
	file.write(reinterpret_cast<char*>(&nBricks), sizeof(nBricks));
	for (Brick* brick : bricks) {
		brick->Save(file);
	}

	file.close();
	return messageFile;
}

BrickGrid::MessageFile BrickGrid::DeleteBrickGrid(std::string filename)
{
	BrickGrid::PrepareFilename(filename);
	return std::filesystem::remove(filename) ? MessageFile::Deleted : MessageFile::Error;
}

void BrickGrid::Draw(Graphics& gfx) const
{
	for (Brick* b : bricks) {
		b->Draw(gfx);
	}
}

std::pair<void*, int> BrickGrid::CheckBallCollision(const Ball& ball) const
{
	void* pBrickCollisionHappened = nullptr;
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
	if (BreakableBrick* brick = dynamic_cast<BreakableBrick*>(bricks[brickIndex])) 
	{
		if (*pDestroyed = brick->IsDestroyed()) 
		{
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

constexpr void BrickGrid::SetFilenameBat(std::string& filename)
{
	std::string::size_type pos = filename.find_last_of(".");
	if (pos != std::string::npos) {
		filename.erase(pos);
	}
	filename += ".dat";
}

constexpr void BrickGrid::PrepareFilename(std::string& filename)
{
	if (filename.empty()) {
		filename = "default";
	}
	filename = directory.data() + filename;
	SetFilenameBat(filename);
}

