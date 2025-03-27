#pragma once
#include "Rect.h"
#include "Graphics.h"
#include "Vec2.h"

class PowerUp 
{
public:
	enum class Type
	{
		AddBall,
		BiggerPaddle,
		DoubleBalls,
		Count
	};
public:
	PowerUp(Type type, Vec2 pos, int size, float speed);
	void Draw(Graphics& gfx) const;
	void Update(float dt);
	RectF GetRect() const;
	Type GetType() const;
private:
	Type type;
	Vec2 pos;
	int size;
	float speed;
};