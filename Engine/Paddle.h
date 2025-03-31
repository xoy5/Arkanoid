#pragma once

#include "Keyboard.h"
#include "Graphics.h"
#include "Colors.h"
#include "Rect.h"
#include "Vec2.h"

#include "Attributes.h"
#include "Ball.h"

class Paddle
{
public:
	explicit Paddle(const Vec2& pos, float speed = 300.0f, int width = 110, int height = 30.0f, const Color& color = Colors::White);
	explicit Paddle(float speed = 300.0f, int width = 110, int height = 30, const Color& color = Colors::White);
	void Draw(Graphics& gfx) const;
	void Update(float dt, const Keyboard& kbd);
	void SetAttributesToDefault();
	void SetSpeed(float speed);
	void SetWidth(int width);
	void SetColor(const Color& color);
	bool DoBallCollision(Ball& ball) const;
	void DoWallCollision(const RectF& walls);
	int GetHeight() const;
	int GetWidth() const;

	void GrowWidth();

	RectF GetRect() const;
private:
	Vec2 pos;
	const PaddleAttributes defaultAttr;
	PaddleAttributes attr;
};