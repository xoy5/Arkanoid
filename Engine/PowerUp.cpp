#include "PowerUp.h"

PowerUp::PowerUp(Type type, Vec2 pos, int size, float speed)
	:
	type(type),
	pos(pos),
	size(size),
	speed(speed)
{}

void PowerUp::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), Colors::Red);
}

void PowerUp::Update(float dt)
{
	pos += Vec2{ 0.0f,  1.0f * speed * dt };
}

RectF PowerUp::GetRect() const
{
	return RectF(pos, (float)size, (float)size);
}

PowerUp::Type PowerUp::GetType() const
{
	return type;
}
