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
	BrickGrid(Game& game, int brickGridWidth = 600, int nRowBricks = 8);
	~BrickGrid();
	BrickGrid(const BrickGrid&) = delete;
	BrickGrid& operator=(const BrickGrid&) = delete;
	void Draw(Graphics& gfx) const;
	BrickGrid::MessageFile Load(std::string filename = "default.dat");
	BrickGrid::MessageFile Save(std::string filename = "default.dat");
	BrickGrid::MessageFile DeleteBrickGrid(std::string filename);

public:
	void AddBrickToGrid(Brick* newBrick);
	std::pair<void*, int> CheckBallCollision(const Ball& ball) const;
	void ExecuteBallCollision(Ball& ball, int BrickIndex, Vec2* pHitPos = nullptr, bool* pDestroyed = nullptr);
	static Brick* CreateBrick(Brick::Type type, const RectF& rect = {0, brickWidth, 0, brickHeight});
	static int GetBrickWidth();
	static int GetBrickHeight();
	void ClearBrickGrid();
	static RectF GetRectBrickForRoundPos(Vei2 posMouse);

private:
	void UpdateBrickColor(BreakableBrick* pBrick);
	static constexpr Color GetColorByHp(int i);
	static constexpr void SetFilenameBat(std::string& filename);
	static constexpr void PrepareFilename(std::string& filename);

private:
	Game& game;
	std::vector<Brick*> bricks;
	static constexpr  int brickWidth = 35;
	static constexpr int brickHeight = 15;
	static constexpr int gapX = 1;
	static constexpr int gapY = 1;
	static constexpr int topOffset = 15;
	static constexpr Color colorsBricks[] = { Colors::Red, Colors::Grapefruit, Colors::Pink, Colors::Purple, Colors::Green, Colors::Yellow, Colors::Blue, Colors::Cyan };
	static constexpr int colorsBricksSize = 8;
	static constexpr std::string_view directory = "Files/BrickGrid/";
};
