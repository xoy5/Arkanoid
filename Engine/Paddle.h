#pragma once
#include "Keyboard.h"
#include "Graphics.h"
#include "Colors.h"
#include "Rect.h"
#include "Vec2.h"
#include "Sprite.h"

class Ball;
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
	explicit Paddle(Player player, const std::string& filenamePaddleSprites, const Vec2& pos, const Color& color = Colors::White, Size size = Size::Small);
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
    static float GetHeight();
	float GetWidth() const;
	RectF GetRect() const;

private:
	Sprite sprite;
	Player player;
	Vec2 posCenter;
	Vec2 vel = Vec2{0.0f, 0.0f};
	Color color;
	Size size;

	static constexpr float smWidth = 80.0f;
	static constexpr float mdWidth = 90.0f;
	static constexpr float lgWidth = 100.0f;

	float speed = 600.0f;
	float width = smWidth;
	static constexpr float height = 24.0f;

	static constexpr RectI srcRectSmSprite = {0, (int)smWidth, 0, (int)height};
	static constexpr RectI srcRectMdSprite = { (int)srcRectSmSprite.right, (int)srcRectSmSprite.right + (int)mdWidth, 0, (int)height };
	static constexpr RectI srcRectLgSprite = { (int)srcRectMdSprite.right, (int)srcRectMdSprite.right + (int)lgWidth, 0, (int)height };

	static constexpr float exitXFactor = 0.03f; // percent
};