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
	if (!ball.GetPaddleCooldown())
	{
		const RectF rect = GetRect();
		if (rect.IsOverlappingWith(ball.GetRect()))
		{
			const Vec2 ballPos = ball.GetPosCenter();
			const RectF ballRect = ball.GetRect();
			if (std::signbit(vel.x) != std::signbit(ball.GetVelocity().x)
				||
				( rect.right > ballRect.left || rect.left < ballRect.right ))
			{
				if (ballPos.x < rect.left || rect.right < ballPos.x) {
					ball.ReboundX();
				}
				else{
					float halfSizePaddle = (rect.right - rect.left) / 2.0f;
					float a = (ballPos.x - (rect.left + halfSizePaddle)) / halfSizePaddle;
					ball.SetDirection( Vec2{ a * 2.0f, -1.0f } );
				}
			}
			
			ball.SetLastObjectReboundPtr(this);
			ball.SetPaddleCooldown();
			return true;
		}
	}

	return false;
}

void Paddle::DoWallCollision(const RectF& walls)
{
	const RectF rect = GetRect();

	if (rect.left < walls.left)
	{
		posCenter.x += walls.left - rect.left;
	}
	else if (rect.right > walls.right)
	{
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