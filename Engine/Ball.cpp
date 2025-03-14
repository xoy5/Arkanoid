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


void Ball::BounceBoth()
{
	dir = -dir;
}
void Ball::BounceX()
{
	dir = Vec2{ -dir.x, dir.y };
}
void Ball::BounceY()
{
	dir = Vec2{ dir.x, -dir.y };
}

void Ball::ReflectFromBricksAndWalls(const RectI& rectI)
{
	Vei2 posCenter = Vei2(this->posCenter);
	if (posCenter.x < rectI.left || posCenter.x > rectI.right) {
		BounceX();
	}
	else {
		BounceY();
	}
}

void Ball::RecflectFromPaddle(const RectF& paddleRectF)
{
	if (posCenter.x < paddleRectF.left || posCenter.x > paddleRectF.right) {
		BounceX();
	}
	else {
		float halfSizePaddle = (paddleRectF.right - paddleRectF.left) / 2.0f;
		float a = (posCenter.x - (paddleRectF.left + halfSizePaddle)) / halfSizePaddle;
		dir = Vec2{ a * 2.0f, -1.0f }.GetNormalized();
	}
}

RectF Ball::GetRectF() const
{
	return RectF::FromCenter(posCenter, attr.radius, attr.radius);
}
Vec2 Ball::GetPosCenter() const
{
	return posCenter;
}
