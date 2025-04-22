#pragma once

#include "Surface.h"
#include "SpriteEffect.h"
#include "Graphics.h"
#include <vector>

class Animation
{
public:
	Animation() = default;
	Animation(int x, int y, int width, int height, int count, const Surface* sprite, float holdTime, Color chroma = Colors::Magenta);
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
private:
	void Advance();
private:
	Color chroma;
	const Surface* sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
};