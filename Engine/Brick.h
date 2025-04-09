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
		Unbreakable
	};

public:
	Brick() = default;
	Brick(const RectF& rect);
	virtual void Draw(Graphics& gfx) const = 0;
	virtual void Save(std::ofstream& file) const;
	virtual void Load(std::ifstream& file);

public:
	void SetPos(const Vec2& pos);
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
	BreakableBrick(const RectF& rect, int hp, const Color& color);
	void Draw(Graphics& gfx) const override;
	void Save(std::ofstream& file) const override;
	void Load(std::ifstream& file) override;

public:
	void Hitted() override;
	bool IsDestroyed() const;
	void SetColor(const Color& color);
	int GetHp() const;

private:
	int hp = 0;
	Color color = Colors::Black;
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
};
