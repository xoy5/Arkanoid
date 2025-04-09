#include "Paddle.h"

Paddle::Paddle(const Vec2& posCenter, float speed, int width, int height, const Color& color)
	:
	posCenter(posCenter),
	defaultAttr(PaddleAttributes{ speed, width, height, color }),
	attr(defaultAttr)
{}

Paddle::Paddle(float speed, int width, int height, const Color& color)
	:
	posCenter(Vec2{float(Graphics::ScreenWidth / 2), float(Graphics::ScreenHeight - Graphics::ScreenHeight / 8)}),
	defaultAttr(PaddleAttributes{ speed, width, height, color }),
	attr(defaultAttr)
{}

void Paddle::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), attr.color);
}

void Paddle::Update(float dt, const Keyboard& kbd)
{
	float xDir = 0.0f;
	if (kbd.KeyIsPressed('A')) xDir += -1.0f;
	if (kbd.KeyIsPressed('D')) xDir += 1.0f;

	vel = Vec2{ xDir * attr.speed, 0 };
	posCenter += vel * dt;
}

void Paddle::SetAttributesToDefault()
{
	attr = defaultAttr;
}

void Paddle::SetSpeed(float speed)
{
	attr.speed = speed;
}

void Paddle::SetWidth(int width)
{
	attr.width = width;
}

void Paddle::SetColor(const Color& color)
{
	attr.color = color;
}

bool Paddle::DoBallCollision(Ball& ball) const
{
	if (ball.GetPaddleCooldown()) return false;
	const RectF paddleRect = GetRect();
	const RectF ballRect = ball.GetRect();
	if (paddleRect.IsOverlappingWith(ballRect) == false) return false;

	const Vec2 ballCenter = ball.GetPosCenter();
	const Vec2 ballVel = ball.GetVelocity();

	const bool oppositeX = std::signbit(vel.x) != std::signbit(ballVel.x);
	const bool ballOverlappingSides =
		ballRect.right > paddleRect.left && ballRect.left < paddleRect.right;

	if (oppositeX || ballOverlappingSides)
	{
		// x collision
		if (ballCenter.x < paddleRect.left || ballCenter.x > paddleRect.right) {
			ball.DoBrickPrecisionMoveX(paddleRect);
			ball.ReboundX();
		}
		else {
			// y collision
			const float halfWidth = (paddleRect.right - paddleRect.left) / 2.0f;
			const float distanceFromCenter = ballCenter.x - (paddleRect.left + halfWidth);
			const float normalized = distanceFromCenter / halfWidth;

			ball.DoBrickPrecisionMoveY(paddleRect);
			ball.SetDirection(Vec2{ normalized * 2.0f, -1.0f });
		}
	}

	ball.SetLastObjectReboundPtr(this);
	ball.SetPaddleCooldown();
	return true;
}

void Paddle::DoWallCollision(const RectF& walls)
{
	const RectF rect = GetRect();

	if (rect.left < walls.left){
		posCenter.x += walls.left - rect.left;
	}
	else if (rect.right > walls.right){
		posCenter.x -= rect.right - walls.right;
	}
}

int Paddle::GetHeight() const
{
	return attr.height;
}

int Paddle::GetWidth() const
{
	return attr.width;
}

void Paddle::GrowWidth()
{
	attr.width = std::min(int(attr.width * 1.1f), 300);
}

RectF Paddle::GetRect() const
{
	return RectF::FromCenter(posCenter, float(attr.width) / 2.0f, float(attr.height) / 2.0f);
}