#include "Ball.h"
#include "Paddle.h"
#include "SpriteEffect.h"
#include "assert.h"

Ball::Ball(const Sprite* pSprite, const Vec2& posCenter, bool onPaddle, float speed, float radius, Paddle::Player lastPlayerRebound)
	:
	pSprite(pSprite),
	posCenter(posCenter),
	speed(speed),
	radius(radius),
	vel(Vec2{ 0.0f, (onPaddle == false ? 1.0f : (lastPlayerRebound == Paddle::Player::Player1 ? -1.0f : 1.0f)) * speed}),
	lastPlayerRebound(lastPlayerRebound)
{
	assert(!(onPaddle == true && lastPlayerRebound == Paddle::Player::None));
}

void Ball::Draw(Graphics& gfx) const
{
	RectI rect = RectI(GetRect());
	gfx.DrawSprite(rect.left, rect.top, *pSprite, SpriteEffect::Chroma(Colors::Magenta));
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
	// left
	if (vel.x < 0) {
		posCenter.x = rect.right + radius;
	}
	// right
	else if (vel.x > 0) {
		posCenter.x = rect.left - radius;
	}
}

void Ball::DoBrickPrecisionMoveY(const RectF& rect)
{
	RectF ballRect = GetRect();
	// top
	if (vel.y > 0) {
		posCenter.y = rect.top - radius;
	}
	// bottom
	else if (vel.y < 0) {
		posCenter.y = rect.bottom + radius;
	}
}

Ball::WallHit Ball::DoWallCollision(const RectF& walls)
{
	WallHit hit = WallHit::NoWallHit;
	const RectF rect = GetRect();
	if (rect.left < walls.left) {
		hit = WallHit::WallHit;
		posCenter.x += walls.left - rect.left;
		ReboundX();
	}
	else if (rect.right > walls.right) {
		hit = WallHit::WallHit;
		posCenter.x -= rect.right - walls.right;
		ReboundX();
	}

	if (rect.top < walls.top) {
		hit = WallHit::TopWallHit;
		posCenter.y += walls.top - rect.top;
		ReboundY();
	}
	else if (rect.bottom > walls.bottom) {
		hit = WallHit::BottomWallHit;
		posCenter.y -= rect.bottom - walls.bottom;
		ReboundY();
	}

	return hit;
}

void Ball::SetDirection(const Vec2& dir)
{
	vel = dir.GetNormalized() * speed;
}

///// Setters and Getters /////

RectF Ball::GetRect() const
{
	RectF rect = RectF::FromCenter(posCenter, radius, radius);
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

void Ball::SetLastObjectReboundPtr(const void* pObjectRebound)
{
	pLastObjectRebound = pObjectRebound;
}

const void* Ball::GetLastObjectReboundPtr() const
{
	return pLastObjectRebound;
}

void Ball::SetLastPlayerRebound(Paddle::Player player)
{
	lastPlayerRebound = player;
}

Paddle::Player Ball::GetLastPlayerRebound() const
{
	return lastPlayerRebound;
}
