#pragma once
#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include "Rect.h"
#include "Attributes.h"

class Ball
{
public:
	enum class WallHit
	{
		NoWallHit,
		WallHit,
		BottomWallHit
	};
public:
	explicit Ball(const Vec2& posCenter, bool onPaddle, float speed = 180.0f, int radius = 6, const Color& color = Colors::White);
	explicit Ball(float speed = 180.0f, int radius = 6, const Color& color = Colors::White);
	void Draw(Graphics& gfx) const;
	void Update(float dt);
	void UpdateByPaddleX(float x);
	void ReboundX();
	void ReboundY();

	void DoBrickPrecisionMoveX(const RectF& rect);
	void DoBrickPrecisionMoveY(const RectF& rect);

	WallHit DoWallCollision(const RectF& walls);
	void SetDirection(const Vec2& dir);

	void ResetPaddleCooldown();
	void SetPaddleCooldown();
	bool GetPaddleCooldown() const;

	RectF GetRect() const;
	Vec2 GetPosCenter() const;
	Vec2 GetVelocity() const;
	
	void SetLastObjectReboundPtr(const void* pObjectRebound);
	const void* GetLastObjectReboundPtr() const;
private:
	float speed;
	int radius;
	Color color;
	Vec2 posCenter;
	Vec2 vel;
	bool isPaddleCooldown = false;
	const void* pLastObjectRebound = nullptr;
};
