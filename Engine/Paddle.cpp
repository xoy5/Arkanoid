#include "Paddle.h"

Paddle::Paddle(Player player, const Vec2& posCenter, const Color& color)
	:
	player(player),
	posCenter(posCenter),
	color(color)
{
}


void Paddle::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), color);
}

void Paddle::Update(float dt, const Keyboard& kbd)
{
	float xDir = 0.0f;
	if (player == Player::Player1) {
		if (kbd.KeyIsPressed('A')) xDir += -1.0f;
		if (kbd.KeyIsPressed('D')) xDir += 1.0f;
	}
	else {
		if (kbd.KeyIsPressed(VK_LEFT)) xDir += -1.0f;
		if (kbd.KeyIsPressed(VK_RIGHT)) xDir += 1.0f;
	}

	vel = Vec2{ xDir * speed, 0.0f };
	posCenter += vel * dt;
}

bool Paddle::DoBallCollision(Ball& ball) const
{
	if (ball.GetLastObjectReboundPtr() == this) return false;
	const RectF paddleRect = GetRect();
	const RectF ballRect = ball.GetRect();
	if (paddleRect.IsOverlappingWith(ballRect) == false) return false;
	const Vec2 ballPosCenter = ball.GetPosCenter();
	const Vec2 ballVel = ball.GetVelocity();

	float yDir = std::signbit(-ballVel.y) ? -1.0f : 1.0f;
	// y collision
	if ( ( std::signbit(ballVel.x) == std::signbit((ballPosCenter - posCenter).x) && ballVel.x != 0.0f)
		|| (ballPosCenter.x >= paddleRect.left && ballRect.right <= paddleRect.right)){
		Vec2 dir;
		const float xDifference = ballPosCenter.x - posCenter.x;
		const float fixedZoneHalfWidth = width / 8.0f;
		const float fixedXComponent = fixedZoneHalfWidth * exitXFactor;
		if (std::abs(xDifference) < fixedZoneHalfWidth)
		{
			dir = Vec2((xDifference < 0.0f ? -1.0f : 1.0f) * fixedXComponent, yDir);
		}
		else
		{
			dir = Vec2(xDifference * exitXFactor, yDir);
		}
		ball.SetDirection(dir);
	}
	// x collision
	else {
		if (ballVel.x == 0.0f) {
			ball.SetDirection(Vec2((std::signbit(ballPosCenter.x - posCenter.x) ? -1.0f : 1.0f), -yDir));
		}
		else {
			ball.ReboundX();
		}
	}

	ball.SetLastObjectReboundPtr(this);
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

void Paddle::GrowWidth()
{
	size = Size(std::min((int)Size::Large, (int)size + 1));
}

///// Setter and Getters /////
void Paddle::SetSpeed(float speed)
{
	this->speed = speed;
}

void Paddle::SetWidth(float width)
{
	this->width = width;
}

void Paddle::SetColor(const Color& color)
{
	this->color = color;
}


float Paddle::GetHeight() const
{
	return height;
}

float Paddle::GetWidth() const
{
	return width;
}


RectF Paddle::GetRect() const
{
	return RectF::FromCenter(posCenter, width / 2.0f, height / 2.0f);
}