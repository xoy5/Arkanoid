#pragma once
#include "Graphics.h"
#include "Font.h"
#include "Rect.h"

class GameStats
{
public:
	GameStats(const Font* font, const RectF& rect, int hp);
	void Draw(Graphics& gfx);
	void Update(float dt);

public:
	void SetTime(float time);
	void HpReset();
	void HpSubtract();
	int GetHp() const;

private:
	const Font* font;
	const RectF rect;
	const int hpMax;
	int hp;
	int points;
	float time;
};