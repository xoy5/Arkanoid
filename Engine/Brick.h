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
	virtual void Hitted() = 0;
	RectF GetRectF() const;
	Vec2 GetPosCenter() const;

	virtual void Save(std::ofstream& file) const;
	virtual void Load(std::ifstream& file);
protected:
	RectF rect = RectF{ 0,0,0,0 };
};

class BreakableBrick : public Brick
{
public:
	BreakableBrick() = default;
	BreakableBrick(const RectF& rect, int hp, const Color& color);
	void Draw(Graphics& gfx) const override;
	void Hitted() override;
	bool IsDestroyed() const;
	void SetColor(const Color& color);
	int GetHp() const;

	void Save(std::ofstream& file) const override;
	void Load(std::ifstream& file) override;
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
	void Hitted() override;

	void Save(std::ofstream& file) const override;
	void Load(std::ifstream& file) override;
};
