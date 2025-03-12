#pragma once
#include "Rect.h"
#include "Graphics.h"
#include "Colors.h"

class Brick
{
public:
	Brick(const RectI& rect, const Color& color, int hp);
	void Draw(Graphics& gfx) const;
private:
	RectI rect;
	Color color;
	int hp;
};