#pragma once

#include "Graphics.h"
#include "Colors.h"
#include "Rect.h"


class Brick
{
public:
	Brick(const RectF& rect);
	virtual void Draw(Graphics& gfx) const = 0;
	virtual void Hitted() = 0;
	RectF GetRectF() const;
	Vec2 GetPosCenter() const;
protected:
	RectF rect;
};

class BreakableBrick : public Brick
{
public:
	BreakableBrick(const RectF& rect, const Color& color, int hp);
	void Draw(Graphics& gfx) const override;
	void Hitted() override;
	bool IsDestroyed() const;
private:
	Color color;
	int hp;
};

class UnbreakableBrick : public Brick
{
public:
	UnbreakableBrick(const RectF& rect);
	void Draw(Graphics& gfx) const override;
	void Hitted() override;
};
