#pragma once
#include <vector>
#include <assert.h>
#include <fstream>
#include <string>

#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"

#include "Brick.h"
#include "Attributes.h"
#include "Ball.h"
#include "Button.h"


class BrickGrid
{
private:
	enum class Message
	{
		NoMessage,
		FileAlreadyExists,
		FileNotExists,
	};
public:
	BrickGrid(int brickGridWidth = 600, int topOffset = 15, int paddingX = 4, int paddingY = 4, int widthBrick = 80, int heightBrick = 30, int nRowBricks = 8);
	~BrickGrid();
	
	void Load(std::string filename = "default.dat");
	void Save(std::string filename = "default.dat");
	Message GetMyMessage() const;

	void Draw(Graphics& gfx) const;

	std::pair<void*, int> CheckBallCollision(const Ball& ball) const;
	void ExecuteBallCollision(Ball& ball, int BrickIndex, Vec2* pHitPos = nullptr, bool* pDestroyed = nullptr);

private:
	void UpdateBrickColor(BreakableBrick* pBrick);
	Color GetColorByHp(int i) const;
	void SetFilenameBat(std::string& filename) const;

private:
	std::vector<Brick*> bricks;
	Message message = Message::NoMessage;
	constexpr static Color colorsBricks[] = { Colors::Red, Colors::Grapefruit, Colors::Pink, Colors::Purple, Colors::Green, Colors::Yellow, Colors::Blue, Colors::Cyan };
	constexpr static int colorsBricksSize = 8;
};
