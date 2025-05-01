#pragma once
#include "Graphics.h"

#include "string"

#include "Font.h"
#include "Rect.h"

class GameStats
{
public:
	GameStats(const Font* font, const std::string& filenameRecords, const RectI& rect, int hp);
	void Draw(Graphics& gfx) const;
	void Update(float dt);

public:
	void AddPoints(int points);
	void HpReset();
	void HpSubtract();
	int GetHp() const;

private:
	const Font* font;
	const int charWidth;
	const int charHeight;
	const int fullChars;
	const int restPixels;
	const RectI rect;
	const int hpMax;
	int hp;
	int recordScore;
	int score = 0;
	int round = 1;
	float time = 0.0f;
};