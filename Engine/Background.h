#pragma once
#include <string>

#include "Rect.h"
#include "Graphics.h"
#include "Sprite.h"
#include "SpriteEffect.h"
#include "Animation.h"

class Background
{
public:
	Background(const std::string& filenamePipe, const std::string& filenamePipeExtra, const std::string& filenamePipeAnimationOppening, /*const std::string& filenamePipeAnimationOppening,*/ const RectF& walls)
		:
		pipe(filenamePipe),
		pipeExtra(filenamePipeExtra),
		pipeSpriteAnimationOppening(filenamePipeAnimationOppening),
		pipeAnimationOppening(0,0, pipesWidth, pipesHeight, 10, &pipeSpriteAnimationOppening, 0.1f, Colors::Magenta),
		walls(walls)
	{
	}

	void Draw(Graphics& gfx) const
	{
		Vec2 posLeftWall = { walls.left, walls.bottom - pipesHeight / 2 };
		Vec2 posRightWall = { walls.right - pipesWidth, walls.bottom - pipesHeight / 2 };
		for (int i = 0; i < 7; i++) {
			// left
			gfx.DrawSprite(posLeftWall.x + 1, posLeftWall.y, pipe, SpriteEffect::Copy{});
			posLeftWall -= {0.0f, pipesHeight};
			gfx.DrawSprite(posLeftWall.x, posLeftWall.y, pipeExtra, SpriteEffect::Chroma{ Colors::Magenta });
			posLeftWall -= {0.0f, pipesHeight};
			// right
			gfx.DrawSprite(posRightWall.x + 1, posRightWall.y, pipe, SpriteEffect::Copy{});
			posRightWall -= {0.0f, pipesHeight};
			if (i == 0)
			{
				//gfx.DrawSprite(posRightWall.x, posRightWall.y, pipeExtra, SpriteEffect::Chroma{ Colors::Magenta });
				pipeAnimationOppening.Draw(Vei2(posRightWall), gfx, SpriteEffect::Chroma(Colors::Magenta));

			}
				posRightWall -= {0.0f, pipesHeight};
		}

	}

	void Update(float dt)
	{
		pipeAnimationOppening.Update(dt);
	}

private:
	static constexpr int pipesHeight = 50;
	static constexpr int pipesWidth = 20;
	RectF walls;
	Sprite pipe;
	Sprite pipeExtra;
	Sprite pipeSpriteAnimationOppening;
	Animation pipeAnimationOppening;
	/*Animation pipeAnimationLaser;*/
};