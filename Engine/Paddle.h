#pragma once
#include "Keyboard.h"
#include "Graphics.h"
#include "Colors.h"
#include "Rect.h"
#include "Vec2.h"

#include "Ball.h"

class Paddle
{
public:
	enum class Player 
	{
		None,
		Player1,
		Player2
	};
public:
	explicit Paddle(Player player, const Vec2& pos, const Color& color = Colors::White);
	void Draw(Graphics& gfx) const;
	void Update(float dt, const Keyboard& kbd);

public:
	bool DoBallCollision(Ball& ball) const;
	void DoWallCollision(const RectF& walls);
	void GrowWidth();

public:
	void SetSpeed(float speed);
	void SetWidth(float width);
	void SetColor(const Color& color);
	float GetHeight() const;
	float GetWidth() const;
	RectF GetRect() const;

private:

	Player player;
	Vec2 posCenter;
	Vec2 vel = Vec2{0.0f, 0.0f};
	Color color;

	static constexpr float fixedWidth = 80;
	static constexpr float fixedHeight = 24;

	float speed = 600.0f;
	float width = fixedWidth;
	float height = fixedHeight;
	const float maxWidth;

	static constexpr float exitXFactor = 0.02f; // percent
};