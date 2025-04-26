#pragma once

#include "Sprite.h"
#include "SpriteEffect.h"
#include "Graphics.h"
#include <vector>

class Animation
{
public:
	Animation() = default;
	Animation(int x, int y, int width, int height, int count, const Sprite* sprite, float holdTime, Color chroma = Colors::Magenta);
	template<typename E>
	void Draw(const Vei2& pos, Graphics& gfx, E effect) const
	{
		gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], *sprite, effect);
	};
	template<typename E>
	void Draw(const Vei2& pos, Graphics& gfx, const RectI& clip, E effect) const
	{
		gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], clip, *sprite, SpriteEffect::Ghost{ chroma });
	};
	void Update(float dt);
	int GetFullAnimationCount() const;
	void ResetFullAnimationCount();
	void Reset();
private:
	void Advance();
private:
	Color chroma;
	const Sprite* sprite = nullptr;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	int fullAnimationCount = 0;
	float holdTime;
	float curFrameTime = 0.0f;
};