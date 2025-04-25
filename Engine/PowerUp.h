#pragma once
#include <memory>
#include "Graphics.h"
#include "Vec2.h"
#include "Rect.h"
#include "Surface.h"

class PowerUp
{
public:
	enum class Type
	{
		AddBall,
		GrowWidthPaddle,
		DoubleBalls,
		Count
	};

public:
	PowerUp(std::shared_ptr<Surface> spriteBox, Type type, const Vec2& pos, const Vec2& dir, int size, float speed);
	void Draw(Graphics& gfx) const;
	void Update(float dt);

public:
	RectF GetRect() const;
	Type GetType() const;

private:
	Type type;
	Vec2 pos;
	Vec2 dir;
	int size;
	float speed;
	std::shared_ptr<Surface> spriteBox;
};
