#include "PowerUp.h"
#include "SpriteEffect.h"

PowerUp::PowerUp(std::shared_ptr<Surface> spriteBox, Type type, const Vec2& pos, const Vec2& dir, int size, float speed)
	:
	spriteBox(spriteBox),
	type(type),
	pos(pos),
	dir(dir),
	size(size),
	speed(speed)
{
}

void PowerUp::Draw(Graphics& gfx) const
{
	gfx.DrawSprite(pos.x, pos.y, *spriteBox, SpriteEffect::Copy{});
}

void PowerUp::Update(float dt)
{
	pos += Vec2{ dir.x,  dir.y * speed * dt };
}

RectF PowerUp::GetRect() const
{
	return RectF(pos, (float)size, (float)size);
}

PowerUp::Type PowerUp::GetType() const
{
	return type;
}
