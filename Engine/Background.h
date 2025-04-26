#pragma once
#include <string>

#include "Rect.h"
#include "Graphics.h"
#include "Sprite.h"
#include "SpriteEffect.h"
#include "Animation.h"

class Background
{
private:
	enum class DoorState
	{
		Closed,
		Oppening,
		Laser
	};
public:
	Background(const std::string& filenamePipes, const RectF& walls)
		:
		pipesSprite(filenamePipes),
		pipeAnimationOppening(40, 0, pipesWidth, pipesHeight, 10, &pipesSprite, 0.1f, Colors::Magenta),
		pipeAnimationLaser(240, 0, pipesWidth, pipesHeight, 5, &pipesSprite, 0.05f, Colors::Magenta),
		walls(walls)
	{
	}

	void Draw(Graphics& gfx) const
	{
		gfx.DrawRect(walls, Colors::Green);
		RectF walls = this->walls.GetExpanded(pipesWidth + borderOffset, pipesWidth + borderOffset, 0.0f, pipesWidth + borderOffset);

		Vec2 posLeftWall = { walls.left, walls.bottom - pipesHeight / 2 };
		Vec2 posRightWall = { walls.right - pipesWidth, walls.bottom - pipesHeight / 2 };

		for (int i = 0; i < 7; i++)
		{
			// left
			gfx.DrawSprite(posLeftWall.x, posLeftWall.y, srcRectPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			posLeftWall -= {0.0f, pipesHeight};
			gfx.DrawSprite(posLeftWall.x, posLeftWall.y, srcRectPipeExtra, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			posLeftWall -= {0.0f, pipesHeight};
			// right
			gfx.DrawSprite(posRightWall.x, posRightWall.y, srcRectPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			posRightWall -= {0.0f, pipesHeight};
			if (i == 0)
			{
				switch (doorState)
				{
				case DoorState::Closed:
					gfx.DrawSprite(posRightWall.x, posRightWall.y, srcRectPipeExtra, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
					break;
				case DoorState::Oppening:
					pipeAnimationOppening.Draw(Vei2(posRightWall), gfx, SpriteEffect::Chroma(Colors::Magenta));
					break;
				case DoorState::Laser:
					pipeAnimationLaser.Draw(Vei2(posRightWall), gfx, SpriteEffect::Chroma(Colors::Magenta));
					break;
				}
			}
			else
			{
				gfx.DrawSprite(posRightWall.x, posRightWall.y, srcRectPipeExtra, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			}
			posRightWall -= {0.0f, pipesHeight};
		}


		// swap
		const int pipesWidth = this->pipesHeight;
		const int pipesHeight = this->pipesWidth;

		Vec2 posTopWallLeft = { walls.left, walls.top };
		Vec2 posTopWallRight = { walls.right - pipesCornerWidth, walls.top };

		gfx.DrawSprite(posTopWallLeft.x, posTopWallLeft.y, srcRectTopLeftCornerPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
		gfx.DrawSprite(posTopWallRight.x, posTopWallRight.y, srcRectTopRightCornerPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
		posTopWallLeft += { pipesCornerWidth, 0 };
		posTopWallRight -= { pipesWidth, 0 };

		for (int i = 0; i < 2; i++)
		{
			gfx.DrawSprite(posTopWallLeft.x, posTopWallLeft.y, srcRectTopPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			gfx.DrawSprite(posTopWallRight.x, posTopWallRight.y, srcRectTopPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			posTopWallLeft += { pipesWidth, 0 };
			posTopWallRight -= { pipesWidth, 0 };
		}

		gfx.DrawSprite(posTopWallLeft.x, posTopWallLeft.y, srcRectTopPipeExtra, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
		gfx.DrawSprite(posTopWallRight.x, posTopWallRight.y, srcRectTopPipeExtra, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
		posTopWallLeft += { pipesWidth, 0 };
		posTopWallRight -= { pipesWidth, 0 };

		for (int i = 0; i < 3; i++)
		{
			gfx.DrawSprite(posTopWallLeft.x, posTopWallLeft.y, srcRectTopPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			gfx.DrawSprite(posTopWallRight.x, posTopWallRight.y, srcRectTopPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			posTopWallLeft += { pipesWidth, 0 };
			posTopWallRight -= { pipesWidth, 0};
		}

		gfx.DrawSprite(posTopWallLeft.x, posTopWallLeft.y, srcRectTopPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
	}

	void Update(float dt)
	{
		switch (doorState)
		{
		case DoorState::Oppening:
			pipeAnimationOppening.Update(dt);
			if (pipeAnimationOppening.GetFullAnimationCount() >= 1) {
				pipeAnimationOppening.Reset();
				doorState = DoorState::Laser;
			}
			break;
		case DoorState::Laser:
			pipeAnimationLaser.Update(dt);
			break;
		}
	}

private:
	RectF walls;
	DoorState doorState = DoorState::Oppening;
	static constexpr int borderOffset = 4;
	static constexpr int pipesHeight = 50;
	static constexpr int pipesWidth = 20;
	static constexpr int pipesCornerWidth = 20;

	// Left and Right
	static constexpr RectI srcRectPipe = { 0, 20, 0, 50 };
	static constexpr RectI srcRectPipeExtra = { 20, 40, 0, 50 };

	// Top
	static constexpr RectI srcRectTopPipeExtra = { 0, 50, 50, 70 };
	static constexpr RectI srcRectTopPipe = { 0, 50, 70, 90 };
	static constexpr RectI srcRectTopLeftCornerPipe = { 50, 70, 70, 90 };
	static constexpr RectI srcRectTopRightCornerPipe = { 70, 90, 70, 90 };
	static constexpr RectI srcRectTopPipeLong = { 90, 180, 70, 90 };
	Sprite pipesSprite;
	Animation pipeAnimationOppening;
	Animation pipeAnimationLaser;
	Animation topPipeAnimationOppening;
};