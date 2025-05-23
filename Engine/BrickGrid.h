#pragma once
#include <vector>
#include <assert.h>
#include <fstream>
#include <string>

#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include "Button.h"

#include "Brick.h"
#include "Ball.h"

class Game;
class BrickGrid
{
public:
	enum class MessageFile
	{
		NoMessage,
		Saved,
		Loaded,
		AlreadyExists,
		NotExists,
		Deleted,
		Error
	};

public:
	BrickGrid(Game& game, const std::string& dir, const std::string& fileBreakableBrickSprites, const std::string& fileUnbreakableBrickSprite);
	~BrickGrid();
	BrickGrid(const BrickGrid&) = delete;
	BrickGrid& operator=(const BrickGrid&) = delete;
	void Draw(Graphics& gfx) const;
	void Update(float dt);

public:
	BrickGrid::MessageFile Load(const std::string& folder, std::string filename = "default.dat");
	BrickGrid::MessageFile Save(const std::string& folder, std::string filename = "default.dat");
	BrickGrid::MessageFile DeleteBrickGrid(const std::string& folder, std::string filename);
	void ClearBrickGrid();
	void AddBrickToGrid(Brick* newBrick);
	void RemoveFromGrid(const Vei2& posMouse);
	std::pair<void*, int> CheckBallCollision(const Ball& ball) const;
	void ExecuteBallCollision(Ball& ball, int BrickIndex, Vec2* pHitPos = nullptr, bool* pDestroyed = nullptr);
	Brick* CreateBrick(Brick::Type type, const RectF& rect = {0, brickWidth, 0, brickHeight}, const BreakableBrick::Color& color = BreakableBrick::Color::None);

public:
	bool IsRoundFinished() const;
	static int GetBrickWidth();
	static int GetBrickHeight();
	RectF GetRectBrickForRoundPos(Vei2 posMouse);

private:
	static constexpr void SetFilenameBat(std::string& fileSrc);
	void PrepareFilename(const std::string& folder, std::string& filename);

private:
	Game& game;
	const std::string directory;
	std::vector<Brick*> bricks;

	static constexpr  int brickWidth = 55;
	static constexpr int brickHeight = 20;
	static constexpr int nColBricks = 11;

	const Sprite breakableBricksSprites;
	static constexpr int breakableBricksNSprites = 5;
	const Sprite unbreakableBricksSprite;
};