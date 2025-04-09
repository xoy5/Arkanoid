#include "Brick.h"

Brick::Brick(const RectF& rect)
	:
	rect(rect)
{}
RectF Brick::GetRect() const
{
	return rect;
}
void Brick::SetPos(const Vec2& pos)
{
	rect = RectF{pos, GetRect().GetWidth(), GetRect().GetHeight()};
}
Vec2 Brick::GetPosCenter() const
{
	return rect.GetCenter();
}
void Brick::Save(std::ofstream& file) const
{
	file.write(reinterpret_cast<const char*>(&rect), sizeof(rect));
}
void Brick::Load(std::ifstream& file)
{
	file.read(reinterpret_cast<char*>(&rect), sizeof(rect));
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
void BreakableBrick::Save(std::ofstream& file) const
{
	Type breakable = Type::Breakable;
	file.write(reinterpret_cast<char*>(&breakable), sizeof(breakable));	
	Brick::Save(file);
	file.write(reinterpret_cast<const char*>(&hp), sizeof(hp));
	file.write(reinterpret_cast<const char*>(&color), sizeof(color));
}
void BreakableBrick::Load(std::ifstream& file)
{
	Brick::Load(file);
	file.read(reinterpret_cast<char*>(&hp), sizeof(hp));
	file.read(reinterpret_cast<char*>(&color), sizeof(color));
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

void UnbreakableBrick::Save(std::ofstream& file) const
{
	Type unbreakable = Type::Unbreakable;
	file.write(reinterpret_cast<char*>(&unbreakable), sizeof(unbreakable));
	Brick::Save(file);
}

void UnbreakableBrick::Load(std::ifstream& file)
{
	Brick::Load(file);
}