#pragma once

#include "Graphics.h"
#include "Colors.h"
#include "Rect.h"

class Brick
{
public:
	Brick(const RectF& rect, const Color& color, int hp);
	void Draw(Graphics& gfx) const;
	void Hitted();

	RectF GetRectF() const;
	Vec2 GetPosCenter() const;
	int GetHp() const;
private:
	RectF rect;
	Color color;
	int hp;
};