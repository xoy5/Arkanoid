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
	enum class Size
	{
		Small,
		Medium,
		Large
	};
public:
	explicit Paddle(Player player, const Vec2& pos, const Color& color = Colors::White, Size size = Size::Small);
	void Draw(Graphics& gfx) const;
	void Update(float dt, const Keyboard& kbd);

public:
	bool DoBallCollision(Ball& ball) const;
	void DoWallCollision(const RectF& walls);
	void WidthGrow();
	void WidthShrink();

public:
	void SetSpeed(float speed);
	void SetColor(const Color& color);
	float GetHeight() const;
	float GetWidth() const;
	RectF GetRect() const;

private:

	Player player;
	Vec2 posCenter;
	Vec2 vel = Vec2{0.0f, 0.0f};
	Color color;
	Size size;

	static constexpr float smWidth = 80;
	static constexpr float mdWidth = 100;
	static constexpr float lgWidth = 120;
	static constexpr float fixedHeight = 24;

	float speed = 600.0f;
	float width = smWidth;
	float height = fixedHeight;

	static constexpr float exitXFactor = 0.02f; // percent
};