#include "Brick.h"

Brick::Brick(const RectF& rect)
	:
	rect(rect)
{}
RectF Brick::GetRectF() const
{
	return rect;
}
Vec2 Brick::GetPosCenter() const
{
	return rect.GetCenter();
}
//////////////////////////////////////////////////////////////////////////////
BreakableBrick::BreakableBrick(const RectF& rect, int hp, const Color& color)
	:
	Brick(rect),
	hp(hp),
	color(color)
{}
void BreakableBrick::Draw(Graphics & gfx) const
{
	gfx.DrawRect(rect, color);
}
void BreakableBrick::Hitted()
{
	if (!IsDestroyed())
	{
		// sound
		hp -= 1;
	}
}
bool BreakableBrick::IsDestroyed() const
{
	return hp <= 0;
}
void BreakableBrick::SetColor(const Color& color)
{
	this->color = color;
}
int BreakableBrick::GetHp() const
{
	return hp;
}
//////////////////////////////////////////////////////////////////////////////
UnbreakableBrick::UnbreakableBrick(const RectF& rect)
	:
	Brick(rect)
{}

void UnbreakableBrick::Draw(Graphics & gfx) const
{
	gfx.DrawRect(rect, Colors::Gray);
}

void UnbreakableBrick::Hitted()
{
	// sound or effect
}
