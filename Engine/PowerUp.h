#pragma once
#include "Rect.h"
#include "Graphics.h"
#include "Vec2.h"

class PowerUp 
{
public:
	PowerUp(Vec2 pos, int size, float speed);
	void Draw(Graphics& gfx) const;
	void Update(float dt);
	RectF GetRect() const;
private:
	Vec2 pos;
	int size;
	float speed;
};