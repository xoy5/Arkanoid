#include "Paddle.h"

Paddle::Paddle(const Vec2& posCenter, float speed, int width, int height, const Color& color)
	:
	posCenter(posCenter),
	defaultAttr(PaddleAttributes{ speed, width, height, color }),
	attr(defaultAttr),
	maxWidth(int(width * 1.6f))
{
}

Paddle::Paddle(float speed, int width, int height, const Color& color)
	:
	posCenter(Vec2{ float(Graphics::ScreenWidth / 2), float(Graphics::ScreenHeight - Graphics::ScreenHeight / 8) }),
	defaultAttr(PaddleAttributes{ speed, width, height, color }),
	attr(defaultAttr),
	maxWidth(int(width * 1.6f))
{
}

void Paddle::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), attr.color);
}

void Paddle::Update(float dt, const Keyboard& kbd)
{
	float xDir = 0.0f;
	if (kbd.KeyIsPressed('A')) xDir += -1.0f;
	if (kbd.KeyIsPressed('D')) xDir += 1.0f;

	vel = Vec2{ xDir * attr.speed, 0.0f };
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
	const Vec2 ballPosCenter = ball.GetPosCenter();
	const Vec2 ballVel = ball.GetVelocity();

	// y collision
	if (std::signbit(ballVel.x) == std::signbit((ballPosCenter - posCenter).x)
		|| (ballPosCenter.x >= paddleRect.left && ballRect.right <= paddleRect.right)){
		Vec2 dir;
		const float xDifference = ballPosCenter.x - posCenter.x;
		const int fixedZoneHalfWidth = (attr.width / 8);
		const float fixedXComponent = fixedZoneHalfWidth * exitXFactor;
		if (std::abs(xDifference) < fixedZoneHalfWidth)
		{
			dir = Vec2((xDifference < 0.0f ? -1.0f : 1.0f) * fixedXComponent, -1.0f);
		}
		else
		{
			dir = Vec2(xDifference * exitXFactor, -1.0f);
		}
		ball.SetDirection(dir);
	}
	// x collision
	else {
		ball.ReboundX();
	}

	ball.SetLastObjectReboundPtr(this);
	ball.SetPaddleCooldown();
	return true;
}

void Paddle::DoWallCollision(const RectF& walls)
{
	const RectF rect = GetRect();

	if (rect.left < walls.left) {
		posCenter.x += walls.left - rect.left;
	}
	else if (rect.right > walls.right) {
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
	attr.width = std::min(int(attr.width * 1.1f), maxWidth);
}

RectF Paddle::GetRect() const
{
	return RectF::FromCenter(posCenter, float(attr.width) / 2.0f, float(attr.height) / 2.0f);
}