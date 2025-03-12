#include "Brick.h"

Brick::Brick(const RectI& rect, const Color& color, int hp)
	:
	rect(rect),
	color(color),
	hp(hp)
{}

void Brick::Draw(Graphics& gfx) const
{
	gfx.DrawRect(rect, color);
}
