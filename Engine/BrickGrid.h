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
#include "Attributes.h"

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
	BrickGrid(Game& game, const std::string& dir, int brickGridWidth = 600, int nRowBricks = 8);
	~BrickGrid();
	BrickGrid(const BrickGrid&) = delete;
	BrickGrid& operator=(const BrickGrid&) = delete;
	void Draw(Graphics& gfx) const;
	BrickGrid::MessageFile Load(std::string filename = "default.dat");
	BrickGrid::MessageFile Save(std::string filename = "default.dat");
	BrickGrid::MessageFile DeleteBrickGrid(std::string filename);
	void ClearBrickGrid();
	void AddBrickToGrid(Brick* newBrick);
	std::pair<void*, int> CheckBallCollision(const Ball& ball) const;
	void ExecuteBallCollision(Ball& ball, int BrickIndex, Vec2* pHitPos = nullptr, bool* pDestroyed = nullptr);

public:
	static Brick* CreateBrick(Brick::Type type, const RectF& rect = {0, brickWidth, 0, brickHeight});
	static int GetBrickWidth();
	static int GetBrickHeight();
	static RectF GetRectBrickForRoundPos(Vei2 posMouse);

private:
	void UpdateBrickColor(BreakableHpBrick* pBrick);
	static constexpr Color GetColorByHp(int i);
	static constexpr void SetFilenameBat(std::string& filename);
	void PrepareFilename(std::string& filename);

private:
	Game& game;
	std::vector<Brick*> bricks;
	static constexpr  int brickWidth = 35;
	static constexpr int brickHeight = 15;
	static constexpr int gapX = 1;
	static constexpr int gapY = 1;
	static constexpr int topOffset = 15;
	static constexpr Color colorsBricks[] = { Colors::Red, Colors::Orange, Colors::Yellow, Colors::GreenYellow, Colors::Green };
	static constexpr int colorsBricksSize = 5;
	const std::string directory;
};
