#include "Ball.h"

Ball::Ball(const Vec2& posCenter, bool onPaddle, float speed, int radius, const Color& color)
	:
	attr({ speed, radius, color }),
	posCenter(posCenter),
	isStillAddedOnPaddle(onPaddle),
	vel(Vec2{ 0.0f, (onPaddle ? -1.0f : 1.0f) * attr.speed })
{}
Ball::Ball(float speed, int radius, const Color& color)
	:
	attr({ speed, radius, color }),
	posCenter(Vec2{ float((Graphics::ScreenWidth / 2) - (radius / 2)), float(Graphics::ScreenHeight / 5 * 3) }),
	//posCenter(Vec2{190, 500}),
	vel(Vec2{ 0.0f, 1.0f * attr.speed}) 
{}
void Ball::Draw(Graphics & gfx) const
{
	gfx.DrawCircle(posCenter, attr.radius, attr.color);
}

void Ball::Update(float dt)
{
	posCenter += vel * dt;
}

void Ball::UpdateByPaddleX(float x)
{
	posCenter.x = x;
}


void Ball::ReboundX()
{
	vel.x = -vel.x;
}
void Ball::ReboundY()
{
	vel.y = -vel.y;
}

void Ball::DoBrickPrecisionMoveX(const RectF& rect)
{
	RectF ballRect = GetRect();
	if (ballRect.left < rect.left) {
		posCenter.x -= ballRect.left - rect.left;
	}
	else if (ballRect.right > rect.right) {
		posCenter.x -= ballRect.right - rect.right;
	}
}

void Ball::DoBrickPrecisionMoveY(const RectF& rect)
{
	RectF ballRect = GetRect();
	if (ballRect.top < rect.top) {
		posCenter.y -= ballRect.top - rect.top;
	}
	else if (ballRect.bottom > rect.bottom) {
		posCenter.y -= ballRect.bottom - rect.bottom;
	}
}

Ball::WallHit Ball::DoWallCollision(const RectF& walls)
{
	WallHit hit = WallHit::NoHit;
	const RectF rect = GetRect();
	if (rect.left < walls.left)
	{
		hit = WallHit::WallHit;
		posCenter.x += walls.left - rect.left;
		ReboundX();
	} 
	else if (rect.right > walls.right)
	{
		hit = WallHit::WallHit;
		posCenter.x -= rect.right - walls.right;
		ReboundX();
	}
	
	if (rect.top < walls.top)
	{
		hit = WallHit::WallHit;
		posCenter.y += walls.top - rect.top;
		ReboundY();
		ResetPaddleCooldown();
	}
	else if (rect.bottom > walls.bottom)
	{
		hit = WallHit::BottomWallHit;
		posCenter.y -= rect.bottom - walls.bottom;
		ReboundY();
	}

	return hit;
}

void Ball::SetDirection(const Vec2& dir)
{
	vel = dir.GetNormalized() * attr.speed;
}

void Ball::ResetPaddleCooldown()
{
	isPaddleCooldown = false;
}

void Ball::SetPaddleCooldown()
{
	isPaddleCooldown = true;
}

bool Ball::GetPaddleCooldown() const
{
	return isPaddleCooldown;
}

RectF Ball::GetRect() const
{
	RectF rect = RectF::FromCenter(posCenter, attr.radius, attr.radius);
	rect.left += 1;
	rect.top += 1; 
	return rect;
}

Vec2 Ball::GetPosCenter() const
{
	return posCenter;
}

Vec2 Ball::GetVelocity() const
{
	return vel;
}

bool Ball::GetIsStillAddedOnPaddle() const
{
	return isStillAddedOnPaddle;
}

void Ball::SetIsStillAddedOnPaddleToFalse()
{
	isStillAddedOnPaddle = false;
}
