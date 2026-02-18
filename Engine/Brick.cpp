#include "Brick.h"
#include <assert.h>

#include "SpriteEffect.h"

Brick::Brick(Type type)
	:
	type(type)
{
}

Brick::Brick(const RectF& rect, const Sprite* sprite, Type type)
	:
	rect(rect),
	sprite(sprite),
	type(type)
{
}

RectF Brick::GetRect() const
{
	return rect;
}

Brick::Type Brick::GetType() const
{
	return type;
}

void Brick::SetPos(const Vec2& pos)
{
	rect = RectF{ pos, GetRect().GetWidth(), GetRect().GetHeight() };
}

void Brick::SetRect(const RectF& rect)
{
	this->rect = rect;
}

Vec2 Brick::GetPosCenter() const
{
	return rect.GetCenter();
}

void Brick::Update(float dt)
{
}

void Brick::Save(std::ofstream& file) const
{
	file.write(reinterpret_cast<const char*>(&rect), sizeof(rect));
}

void Brick::Load(std::ifstream& file, const Sprite* sprite)
{
	file.read(reinterpret_cast<char*>(&rect), sizeof(rect));
	this->sprite = sprite;
}


//////////////////////////////////////////////////////////////////////////////
BreakableBrick::BreakableBrick()
	:
	Brick(Type::Breakable)
{
}

BreakableBrick::BreakableBrick(const RectF& rect, const Sprite* sprite, const RectI& srcRect)
	:
	Brick(rect, sprite, Type::Breakable),
	srcRect(srcRect)
{
}

void BreakableBrick::Draw(Graphics& gfx) const
{
	gfx.DrawSprite(rect.left, rect.top, srcRect, *sprite, SpriteEffect::Copy());
}

void BreakableBrick::DrawGhost(Graphics& gfx) const
{
	gfx.DrawSprite(rect.left, rect.top, srcRect, *sprite, SpriteEffect::Ghost{Colors::Magenta});
}

void BreakableBrick::Save(std::ofstream& file) const
{
	Type breakable = Type::Breakable;
	file.write(reinterpret_cast<char*>(&breakable), sizeof(breakable));
	Brick::Save(file);
	file.write(reinterpret_cast<const char*>(&srcRect), sizeof(srcRect));
}

void BreakableBrick::Load(std::ifstream& file, const Sprite* sprite)
{
	Brick::Load(file, sprite);
	file.read(reinterpret_cast<char*>(&srcRect), sizeof(srcRect));
}

void BreakableBrick::Hitted()
{
	if (!destroyed) {
		// sound
		destroyed = true;
	}
}

bool BreakableBrick::IsDestroyed() const
{
	return destroyed;
}

void BreakableBrick::SetSpriteColor(const RectI& srcRectSpriteColor)
{
	srcRect = srcRectSpriteColor;
}

void BreakableBrick::SetColor(const Color& color)
{
	switch (color)
	{
	case Color::Red:
		srcRect = srcRectRed;
		break;
	case Color::Green:
		srcRect = srcRectGreen;
		break;
	case Color::Blue:
		srcRect = srcRectBlue;
		break;
	case Color::Orange:
		srcRect = srcRectOrange;
		break;
	case Color::Pink:
		srcRect = srcRectPink;
		break;
	default:
		assert(false);
	}
}

RectI BreakableBrick::GetSrcRectSpriteColor(int i)
{
	assert(i >= 0);

	switch (i % nColors)
	{
	case 0:
		return srcRectRed;
	case 1:
		return srcRectGreen;
	case 2:
		return srcRectBlue;
	case 3:
		return srcRectOrange;
	case 4:
		return srcRectPink;
	};
}
//////////////////////////////////////////////////////////////////////////////
BreakableHpBrick::BreakableHpBrick()
	:
	Brick(Type::BreakableHp)
{
}
BreakableHpBrick::BreakableHpBrick(const RectF& rect, int hp, const Color& color)
	:
	Brick(rect, nullptr, Type::BreakableHp),
	hp(hp),
	color(color)
{
}

void BreakableHpBrick::Draw(Graphics& gfx) const
{
	gfx.DrawRect(rect, color);
}

void BreakableHpBrick::DrawGhost(Graphics& gfx) const
{
	gfx.DrawRect(rect, color); // I dont even use that shit
}

void BreakableHpBrick::Save(std::ofstream& file) const
{
	Type breakableHp = Type::BreakableHp;
	file.write(reinterpret_cast<char*>(&breakableHp), sizeof(breakableHp));
	Brick::Save(file);
	file.write(reinterpret_cast<const char*>(&hp), sizeof(hp));
	file.write(reinterpret_cast<const char*>(&color), sizeof(color));
}

void BreakableHpBrick::Load(std::ifstream& file, const Sprite* sprite)
{
	Brick::Load(file, sprite);
	file.read(reinterpret_cast<char*>(&hp), sizeof(hp));
	file.read(reinterpret_cast<char*>(&color), sizeof(color));
}

void BreakableHpBrick::Hitted()
{
	if (!IsDestroyed()) {
		// sound
		hp -= 1;
	}
}

bool BreakableHpBrick::IsDestroyed() const
{
	return hp <= 0;
}

void BreakableHpBrick::SetColor(const Color& color)
{
	this->color = color;
}

int BreakableHpBrick::GetHp() const
{
	return hp;
}
//////////////////////////////////////////////////////////////////////////////
UnbreakableBrick::UnbreakableBrick()
	:
	Brick(Type::Unbreakable)
{
}

UnbreakableBrick::UnbreakableBrick(const RectF& rect, const Sprite* sprite)
	:
	Brick(rect, sprite, Type::Unbreakable),
	animation(55, 0, 55, 20, nFrames, sprite, animationOneFrameTime)
{
}

void UnbreakableBrick::Draw(Graphics& gfx) const
{
	if (activeAnimation) {
		animation.Draw(Vei2(rect.GetPos()), gfx, SpriteEffect::Copy{});
	}
	else {
		gfx.DrawSprite(int(rect.left), int(rect.top), RectI(0, 55, 0, 20), *sprite, SpriteEffect::Copy{});
	}
}

void UnbreakableBrick::DrawGhost(Graphics& gfx) const
{
	if (activeAnimation) {
		animation.Draw(Vei2(rect.GetPos()), gfx, SpriteEffect::Ghost{Colors::Magenta});
	}
	else {
		gfx.DrawSprite(int(rect.left), int(rect.top), RectI(0, 55, 0, 20), *sprite, SpriteEffect::Ghost{Colors::Magenta});
	}
}

void UnbreakableBrick::Update(float dt)
{
	if (activeAnimation)
	{
		animation.Update(dt);
		if (animation.GetFullAnimationCount() >= 1) {
			animation.ResetFullAnimationCount();
			activeAnimation = false;
		}
	}
}

void UnbreakableBrick::Save(std::ofstream& file) const
{
	Type unbreakable = Type::Unbreakable;
	file.write(reinterpret_cast<char*>(&unbreakable), sizeof(unbreakable));
	Brick::Save(file);
}

void UnbreakableBrick::Load(std::ifstream& file, const Sprite* sprite)
{
	Brick::Load(file, sprite);
	animation = Animation(55, 0, 55, 20, nFrames, sprite, animationOneFrameTime);
}

void UnbreakableBrick::Hitted()
{
	// sound or effect
	activeAnimation = true;
}

bool UnbreakableBrick::IsDestroyed() const
{
	return false;
}

constexpr int UnbreakableBrick::GetNFrames()
{
	return nFrames;
}

constexpr float UnbreakableBrick::GetAnimationOneFrameTime()
{
	return animationOneFrameTime;
}
