#include "Ball.h"

Ball::Ball(const Vec2& posCenter, float speed, int radius, const Color& color)
	:
	attr({ speed, radius, color }),
	posCenter(posCenter),
	vel(Vec2{ 0.0f, 1.0f * attr.speed })
{}
Ball::Ball(float speed, int radius, const Color& color)
	:
	attr({ speed, radius, color }),
	posCenter(Vec2{ float((Graphics::ScreenWidth / 2) - (radius / 2)), float(Graphics::ScreenHeight / 5 * 3)}),
	vel({0, 1.0f * attr.speed})
{
}

void Ball::Draw(Graphics & gfx) const
{
	gfx.DrawCircle(posCenter, attr.radius, attr.color);
}

void Ball::Update(float dt)
{
	posCenter += vel * dt;
}


void Ball::ReboundX()
{
	vel.x = -vel.x;
}
void Ball::ReboundY()
{
	vel.y = -vel.y;
}

bool Ball::DoWallCollision(const RectF& walls)
{
	bool collided = false;
	const RectF rect = GetRectF();
	if (rect.left < walls.left)
	{
		collided = true;
		posCenter.x += walls.left - rect.left;
		ReboundX();
	} 
	else if (rect.right > walls.right)
	{
		collided = true;
		posCenter.x -= rect.right - walls.right;
		ReboundX();
	}
	
	if (rect.top < walls.top)
	{
		collided = true;
		posCenter.y += walls.top - rect.top;
		ReboundY();
	}
	else if (rect.bottom > walls.bottom)
	{
		collided = true;
		posCenter.y -= rect.bottom - walls.bottom;
		ReboundY();
	}

	return collided;
}

void Ball::SetDirection(const Vec2& dir)
{
	vel = dir.GetNormalized() * attr.speed;
}

RectF Ball::GetRectF() const
{
	return RectF::FromCenter(posCenter, attr.radius, attr.radius);
}

Vec2 Ball::GetPosCenter() const
{
	return posCenter;
}

Vec2 Ball::GetVelocity() const
{
	return vel;
}
