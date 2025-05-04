#pragma once
#include <fstream>

#include "Graphics.h"
#include "Colors.h"
#include "Rect.h"
#include "Sprite.h"
#include "Animation.h"

class Brick
{
public:
	enum class Type
	{
		Breakable,
		BreakableHp,
		Unbreakable
	};

public:
	Brick() = default;
	Brick(const RectF& rect, const Sprite* sprite);
	virtual void Draw(Graphics& gfx) const = 0;
	virtual void Update(float dt);
	virtual void Save(std::ofstream& file) const;
	virtual void Load(std::ifstream& file, const Sprite* ptr);

public:
	virtual bool IsDestroyed() const = 0;
	void SetPos(const Vec2& pos);
	void SetRect(const RectF& rect);
	virtual void Hitted() = 0;
	Vec2 GetPosCenter() const;
	RectF GetRect() const;

protected:
	RectF rect = RectF{ 0,0,0,0 };
	const Sprite* sprite;
};

class BreakableBrick : public Brick
{
public:
	enum class Color
	{
		Red,
		Green,
		Blue,
		Orange,
		Pink,
		Count, 
		None
	};
public:
	static constexpr RectI srcRectRed = {0, 55, 0, 20};
	static constexpr RectI srcRectGreen = {55, 110, 0, 20};
	static constexpr RectI srcRectBlue = { 110, 165, 0, 20 };
	static constexpr RectI srcRectOrange = { 165, 220, 0, 20 };
	static constexpr RectI srcRectPink = { 220, 275, 0, 20 };
	static constexpr int nColors = 5;
public:
	static RectI GetSrcRectSpriteColor(int i);

public:
	BreakableBrick() = default;
	BreakableBrick(const RectF& rect, const Sprite* sprite, const RectI& srcRect);
	void Draw(Graphics& gfx) const override;
	void Save(std::ofstream& file) const override;
	void Load(std::ifstream& file, const Sprite* sprite) override;

public:
	void Hitted() override;
	bool IsDestroyed() const override;
	void SetSpriteColor(const RectI& srcRectSpriteColor);
	void SetColor(const Color& color);

private:
	bool destroyed = false;
	RectI srcRect;
};

class BreakableHpBrick : public Brick
{
public:
	BreakableHpBrick() = default;
	BreakableHpBrick(const RectF& rect, int hp, const Color& color);
	void Draw(Graphics& gfx) const override;
	void Save(std::ofstream& file) const override;
	void Load(std::ifstream& file, const Sprite* sprite) override;

public:
	void Hitted() override;
	bool IsDestroyed() const override;
	void SetColor(const Color& color);
	int GetHp() const;

private:
	int hp = 0;
	Color color = Colors::White;
};

class UnbreakableBrick : public Brick
{
public:
	UnbreakableBrick() = default;
	UnbreakableBrick(const RectF& rect, const Sprite* sprite);
	void Draw(Graphics& gfx) const override;
	void Update(float dt) override;
	void Save(std::ofstream& file) const override;
	void Load(std::ifstream& file, const Sprite* sprite) override;

public:
	void Hitted() override;
	bool IsDestroyed() const override;

public:
	static constexpr int GetNFrames();
	static constexpr float GetAnimationOneFrameTime();

private:
	bool activeAnimation = false;
	static constexpr float animationTime = 0.6f;
	static constexpr int nFrames = 9;
	static constexpr float animationOneFrameTime = animationTime / nFrames;
	Animation animation;
};