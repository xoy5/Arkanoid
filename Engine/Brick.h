#pragma once

#include "Graphics.h"
#include "Colors.h"
#include "Rect.h"

class Brick
{
public:
	Brick(const RectI& rect, const Color& color, int hp);
	void Draw(Graphics& gfx) const;
	void Hitted();

	RectI GetRectI() const;
	Vei2 GetPosCenter() const;
	int GetHp() const;
private:
	RectI rect;
	Color color;
	int hp;
};