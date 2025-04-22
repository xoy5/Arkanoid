#include "BrickGrid.h"
#include "Game.h"
#include <algorithm>
#include <filesystem>

BrickGrid::BrickGrid(Game& game, const std::string& dir, const std::string& fileBreakableBrickSprites, const std::string& fileUnbreakableBrickSprite, int nRowBricks)
	:
	game(game),
	directory(dir),
	breakableBricksSprites({fileBreakableBrickSprites}),
	unbreakableBricksSprite({fileUnbreakableBrickSprite})
{

	const int brickGridWidth = game.walls.GetWidth();

	int brickGridWHeight = nRowBricks * brickHeight;
	Vec2 gridPos = Vec2{ game.walls.left, game.walls.top };

	Vec2 brickPos = gridPos;

	for (int y = 0; y < nRowBricks; y++, brickPos = Vec2{ gridPos.x, brickPos.y + brickHeight }) {
		if (y == 4)
		{
			for (int x = 0; x < nColBricks; x++, brickPos += Vec2{ brickWidth, 0.0f })
			{
				if (x % 2 == 0) {
					bricks.emplace_back(new UnbreakableBrick(RectF(brickPos, brickPos + Vec2{ brickWidth, brickHeight }), &unbreakableBricksSprite));
				}
			}
		}
		else
		{
			for (int x = 0; x < nColBricks; x++, brickPos += Vec2{ brickWidth, 0.0f }) {
				bricks.emplace_back(new BreakableBrick(RectF(brickPos, brickPos + Vec2{ brickWidth, brickHeight }), &breakableBricksSprites, BreakableBrick::srcRectGreen));
			}
		}
	}

	brickPos = Vec2{ gridPos.x, brickPos.y + brickHeight };
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

	if (!file) {
		messageFile = MessageFile::NotExists;
		return messageFile;
	};

	bricks.clear();
	game.gf_ballManager.ClearBalls();
	game.gf_ballManager.AddBallOnPaddlePlayer1();
	if (game.isTwoPlayerMode) {
		game.gf_ballManager.AddBallOnPaddlePlayer2();
	}

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
			brick->Load(file, &unbreakableBricksSprite);
			break;
		case Brick::Type::Breakable:
			brick = new BreakableBrick;
			brick->Load(file, &breakableBricksSprites);
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

	if (!file.good()) {
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

Brick* BrickGrid::CreateBrick(Brick::Type type, const RectF& rect)
{
	Brick* brick = nullptr;

	switch (type)
	{
	case Brick::Type::Breakable:
		brick = new BreakableBrick(rect, &breakableBricksSprites, BreakableBrick::srcRectGreen);
		break;
	case Brick::Type::BreakableHp:
		brick = new BreakableHpBrick(rect, 5, Colors::Red );
		break;
	case Brick::Type::Unbreakable:
		brick = new UnbreakableBrick(rect, &unbreakableBricksSprite);
		break;
	}

	return brick;
}

///// Setters and Getters /////

int BrickGrid::GetBrickWidth()
{
	return brickWidth;
}

int BrickGrid::GetBrickHeight()
{
	return brickHeight;
}

void BrickGrid::ClearBrickGrid()
{
	bricks.clear();
}

void BrickGrid::Draw(Graphics& gfx) const
{
	for (Brick* b : bricks) {
		b->Draw(gfx);
	}
}

void BrickGrid::Update(float dt)
{
	for (Brick* pBrick : bricks) {
		pBrick->Update(dt);
	}
}

void BrickGrid::AddBrickToGrid(Brick* newBrick)
{
	bricks.emplace_back(newBrick);
}

std::pair<void*, int> BrickGrid::CheckBallCollision(const Ball& ball) const
{
	void* pBrickCollisionHappened = nullptr;
	float minBrickDistSq;
	int brickIndex = -1;
	const Vec2 ballPosCenter = ball.GetPosCenter();

	for (int i = 0; i < bricks.size(); i++)
	{
		if (ball.GetRect().IsOverlappingWith(bricks[i]->GetRect()))
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

	const float epsilon = 0.001f;
	Vec2 ballVelocity = ball.GetVelocity();
	RectF brickRect = bricks[brickIndex]->GetRect();
	Vec2 ballPosCenter = ball.GetPosCenter();

	bool horizontalOverlap = (ballPosCenter.x >= brickRect.left && ballPosCenter.x <= brickRect.right);

	if (fabs(ballVelocity.x) < epsilon) {
		ball.DoBrickPrecisionMoveY(brickRect);
		ball.ReboundY();
	}
	else if (std::signbit(ballVelocity.x) == std::signbit(ballPosCenter.x - bricks[brickIndex]->GetPosCenter().x)) {
		ball.DoBrickPrecisionMoveY(brickRect);
		ball.ReboundY();
	}
	else if (horizontalOverlap) {
		ball.DoBrickPrecisionMoveY(brickRect);
		ball.ReboundY();
	}
	else {
		ball.DoBrickPrecisionMoveX(brickRect);
		ball.ReboundX();
	}

	bricks[brickIndex]->Hitted();
	if (*pDestroyed = bricks[brickIndex]->IsDestroyed())
	{
		if (pHitPos) {
			*pHitPos = bricks[brickIndex]->GetPosCenter();
		}
		delete bricks[brickIndex];
		bricks[brickIndex] = std::move(bricks.back());
		bricks.pop_back();
	}
	else if (BreakableHpBrick* brick = dynamic_cast<BreakableHpBrick*>(bricks[brickIndex]))
	{
		// zmien kolor czy cos
	}
}

constexpr void BrickGrid::SetFilenameBat(std::string& filename)
{
	std::string::size_type pos = filename.find_last_of(".");
	if (pos != std::string::npos) {
		filename.erase(pos);
	}
	filename += ".dat";
}

void BrickGrid::PrepareFilename(std::string& filename)
{
	if (filename.empty()) {
		filename = "default";
	}
	filename = directory + filename;
	SetFilenameBat(filename);
}

RectF BrickGrid::GetRectBrickForRoundPos(Vei2 posMouse)
{
	Vei2 posInGrid = posMouse - Vei2{ 0, topOffset };
	if (posInGrid.x < 0 || posInGrid.y < 0) return RectF{ 0, brickWidth, 0, brickHeight };

	Vei2 addToNext = Vei2{ brickWidth, brickHeight };
	int fullBricksX = posInGrid.x / addToNext.x;
	int fullBricksY = posInGrid.y / addToNext.y;


	enum Pos {
		TopLeft = 0,
		TopRight,
		BottomLeft,
		BottomRight,
		Count
	};

	Vei2 posFrom = Vei2{ 0, topOffset } + Vei2{ addToNext.x * fullBricksX + brickWidth / 2, addToNext.y * fullBricksY + brickHeight / 2 };
	Vei2 positionsCenter[Pos::Count] = {
		posFrom,
		posFrom + Vei2{addToNext.x, 0},
		posFrom + Vei2{0, addToNext.y},
		posFrom + addToNext
	};

	Pos pos = Pos::TopLeft;
	int minLength = (posMouse - positionsCenter[Pos::TopLeft]).GetLengthSq();

	for (int i = 1; i < Pos::Count; i++) {
		if (minLength > (posMouse - positionsCenter[i]).GetLengthSq()) {
			minLength = (posMouse - positionsCenter[i]).GetLengthSq();
			pos = Pos(i);
		}
	}

	return RectF::FromCenter(Vec2(positionsCenter[pos]), brickWidth / 2.0f, brickHeight / 2.0f);
}