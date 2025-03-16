#include "Paddle.h"

Paddle::Paddle(const Vec2& pos, float speed, int width, int height, const Color& color)
	:
	pos(pos),
	defaultAttr(PaddleAttributes{ speed, width, height, color }),
	attr(defaultAttr)
{}

Paddle::Paddle(float speed, int width, int height, const Color& color)
	:
	pos(Vec2{float(Graphics::ScreenWidth / 2) - float(width / 2), float(Graphics::ScreenHeight) - float((Graphics::ScreenHeight) / 8)}),
	defaultAttr(PaddleAttributes{ speed, width, height, color }),
	attr(defaultAttr)
{}

void Paddle::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRectF(), attr.color);
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

bool Paddle::DoBallCollision(Ball& ball) const
{
	const RectF rect = GetRectF();
	if (rect.IsOverlappingWith(ball.GetRectF()))
	{
		const Vec2 ballPos = ball.GetPosCenter();
		if (ballPos.x < rect.left || rect.right < ballPos.x) {
			ball.ReboundX();
		}
		else {
			float halfSizePaddle = (rect.right - rect.left) / 2.0f;
			float a = (ballPos.x - (rect.left + halfSizePaddle)) / halfSizePaddle;
			ball.SetDirection( Vec2{ a * 2.0f, -1.0f } );
		}
		return true;
	}

	return false;
}

void Paddle::DoWallCollision(const RectF& walls)
{
	const RectF rect = GetRectF();

	if (rect.left < walls.left)
	{
		pos.x += walls.left - rect.left;
	}
	else if (rect.right > walls.right)
	{
		pos.x -= rect.right - walls.right;
	}
}

RectF Paddle::GetRectF() const
{
	return RectF{ pos, float(attr.width), float(attr.height) };
}
