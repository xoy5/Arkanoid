#include "Paddle.h"

Paddle::Paddle(const Vec2& pos, float speed, int width, int height, const Color& color)
	:
	pos(pos),
	defaultAttr(PaddleAttributes{ speed, width, height, color }),
	attr(defaultAttr)
{
}

Paddle::Paddle(float speed, int width, int height, const Color& color)
	:
	pos(Vec2{float(Graphics::ScreenWidth / 2) - float(width / 2), float(Graphics::ScreenHeight) - float((Graphics::ScreenHeight) / 8)}),
	defaultAttr(PaddleAttributes{ speed, width, height, color }),
	attr(defaultAttr)
{}

void Paddle::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRectI(), attr.color);
}

void Paddle::Update(float dt, const Keyboard& kbd)
{
	float xDir = 0.0f;
	if (kbd.KeyIsPressed('A')) xDir += -1.0f;
	if (kbd.KeyIsPressed('D')) xDir += 1.0f;

	pos.x += xDir * attr.speed * dt;
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

bool Paddle::IsBallCollision(const Ball& ball) const
{
	return GetRectF().IsOverlappingWith(ball.GetRectF());
}

RectI Paddle::GetRectI() const
{
	return RectI{ Vei2(pos), attr.width, attr.height };
}

RectF Paddle::GetRectF() const
{
	return RectF{ pos, float(attr.width), float(attr.height) };
}
