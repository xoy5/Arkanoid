#pragma once
#include <fstream>

#include "Graphics.h"
#include "Colors.h"
#include "Rect.h"

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
	Brick(const RectF& rect);
	virtual void Draw(Graphics& gfx) const = 0;
	virtual void Save(std::ofstream& file) const;
	virtual void Load(std::ifstream& file);
public:
	virtual bool IsDestroyed() const = 0;
	void SetPos(const Vec2& pos);
	void SetRect(const RectF& rect);
	virtual void Hitted() = 0;
	Vec2 GetPosCenter() const;
	RectF GetRect() const;

protected:
	RectF rect = RectF{ 0,0,0,0 };
};

class BreakableBrick : public Brick
{
public:
	BreakableBrick() = default;
	BreakableBrick(const RectF& rect, const Color& color);
	void Draw(Graphics& gfx) const override;
	void Save(std::ofstream& file) const override;
	void Load(std::ifstream& file) override;

public:
	void Hitted() override;
	bool IsDestroyed() const override;
	void SetColor(const Color& color);

private:
	bool destroyed = false;
	Color color = Colors::White;
};

class BreakableHpBrick : public Brick
{
public:
	BreakableHpBrick() = default;
	BreakableHpBrick(const RectF& rect, int hp, const Color& color);
	void Draw(Graphics& gfx) const override;
	void Save(std::ofstream& file) const override;
	void Load(std::ifstream& file) override;

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
	UnbreakableBrick(const RectF& rect);
	void Draw(Graphics& gfx) const override;
	void Save(std::ofstream& file) const override;
	void Load(std::ifstream& file) override;

public:
	void Hitted() override;
	bool IsDestroyed() const override;
};
