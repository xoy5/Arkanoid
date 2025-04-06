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
	BrickGrid(int brickGridWidth = 600, int topOffset = 15, int paddingX = 4, int paddingY = 4, int widthBrick = 80, int heightBrick = 30, int nRowBricks = 8);
	~BrickGrid();
	BrickGrid(const BrickGrid&) = delete;
	BrickGrid& operator=(const BrickGrid&) = delete;
	void Draw(Graphics& gfx) const;
	BrickGrid::MessageFile Load(std::string filename = "default.dat");
	BrickGrid::MessageFile Save(std::string filename = "default.dat");
	BrickGrid::MessageFile DeleteBrickGrid(std::string filename);
	
public:
	MessageFile GetMessageFile() const;
	void SetMessageFileNoMessage();
	std::pair<void*, int> CheckBallCollision(const Ball& ball) const;
	void ExecuteBallCollision(Ball& ball, int BrickIndex, Vec2* pHitPos = nullptr, bool* pDestroyed = nullptr);

private:
	void UpdateBrickColor(BreakableBrick* pBrick);
	Color GetColorByHp(int i) const;
	static constexpr void SetFilenameBat(std::string& filename);
	static constexpr void PrepareFilename(std::string& filename);

private:
	std::vector<Brick*> bricks;
	MessageFile messageFile = MessageFile::NoMessage;
	constexpr static Color colorsBricks[] = { Colors::Red, Colors::Grapefruit, Colors::Pink, Colors::Purple, Colors::Green, Colors::Yellow, Colors::Blue, Colors::Cyan };
	constexpr static int colorsBricksSize = 8;
	static constexpr std::string_view directory = "Files/BrickGrid/";
};
