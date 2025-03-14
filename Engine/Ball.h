#pragma once
#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include "Rect.h"

#include "Attributes.h"

class Ball
{
public:
	explicit Ball(const Vec2& posCenter, float speed = 300.0f, int radius = 10, const Color& color = Colors::White);
	explicit Ball(float speed = 300.0f, int radius = 10, const Color& color = Colors::White);
	void Draw(Graphics& gfx) const;
	void Update(float dt);
	void BounceBoth();
	void BounceX();
	void BounceY();
	void ReflectFromBricksAndWalls(const RectI& rectI); 
	void RecflectFromPaddle(const RectF& paddleRectF);
	RectF GetRectF() const;
	Vec2 GetPosCenter() const;
private:
	Vec2 posCenter;
	Vec2 dir;
	BallAttributes attr;
};
