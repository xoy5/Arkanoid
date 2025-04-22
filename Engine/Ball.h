#pragma once
#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include "Rect.h"

class Ball
{
public:
	enum class WallHit
	{
		NoWallHit,
		WallHit,
		TopWallHit,
		BottomWallHit
	};

public:
	explicit Ball(const Vec2& posCenter, bool onPaddle, float speed, float radius, const Color& color = Colors::White);
	void Draw(Graphics& gfx) const;
	void Update(float dt);

public:
	void UpdateByPaddleX(float x);
	void ReboundX();
	void ReboundY();
	void DoBrickPrecisionMoveX(const RectF& rect);
	void DoBrickPrecisionMoveY(const RectF& rect);
	WallHit DoWallCollision(const RectF& walls);

public:
	void SetDirection(const Vec2& dir);
	RectF GetRect() const;
	Vec2 GetPosCenter() const;
	Vec2 GetVelocity() const;
	void SetLastObjectReboundPtr(const void* pObjectRebound);
	const void* GetLastObjectReboundPtr() const;

private:
	float speed;
	float radius;
	Color color;
	Vec2 posCenter;
	Vec2 vel;
	const void* pLastObjectRebound = nullptr;
};
