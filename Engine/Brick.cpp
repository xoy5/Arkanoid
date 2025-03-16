#include "Brick.h"

Brick::Brick(const RectF& rect, const Color& color, int hp)
	:
	rect(rect),
	color(color),
	hp(hp)
{}

void Brick::Draw(Graphics& gfx) const
{
	gfx.DrawRect(rect, color);
}

void Brick::Hitted()
{
	hp -= 1;
}

RectF Brick::GetRectF() const
{
	return rect;
}

Vec2 Brick::GetPosCenter() const
{
	return rect.GetCenter();
}

int Brick::GetHp() const
{
	return hp;
}
