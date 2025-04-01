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
	explicit Ball(const Vec2& posCenter, bool onPaddle = false, float speed = 300.0f, int radius = 10, const Color& color = Colors::White);
	explicit Ball(float speed = 300.0f, int radius = 10, const Color& color = Colors::White);
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
	bool GetIsStillAddedOnPaddle() const;
	void SetIsStillAddedOnPaddleToFalse();
	
	void SetLastObjectReboundPtr(void* pObjectRebound);
	void* GetLastObjectReboundPtr() const;
private:
	BallAttributes attr;
	Vec2 posCenter;
	Vec2 vel;
	bool isPaddleCooldown = false;
	bool isStillAddedOnPaddle = false;
	void* pLastObjectRebound = nullptr;
};
