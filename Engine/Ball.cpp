#include "Ball.h"

Ball::Ball(const Vec2& posCenter, float speed, int radius, const Color& color)
	:
	posCenter(posCenter),
	dir(Vec2{ 0.0f, 1.0f }),
	attr({ speed, radius, color })
{}

Ball::Ball(float speed, int radius, const Color& color)
	:
	posCenter(Vec2{ float((Graphics::ScreenWidth / 2) - (radius / 2)), float(Graphics::ScreenHeight / 5 * 3)}),
	dir(Vec2{ 0.0f, 1.0f }),
	attr({ speed, radius, color })
{
}

void Ball::Draw(Graphics & gfx) const
{
	gfx.DrawCircle(Vei2(posCenter), attr.radius, attr.color);
}

void Ball::Update(float dt)
{
	posCenter += dir * attr.speed * dt;
}

void Ball::ChangeDir()
{
	dir = -dir;
}

RectF Ball::GetRectF() const
{
	return RectF::FromCenter(posCenter, attr.radius, attr.radius);
}

Vec2 Ball::GetPosCenter() const
{
	return posCenter;
}
