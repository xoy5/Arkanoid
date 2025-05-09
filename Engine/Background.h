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
	Background(const std::string& filenamePipes, const std::string& filenameBackground, const RectF& walls, int borderOffset)
		:
		pipesSprite(filenamePipes),
		background(filenameBackground),
		pipeAnimationOppening(40, 0, pipesWidth, pipesHeight, 10, &pipesSprite, 0.1f, Colors::Magenta),
		pipeAnimationLaser(240, 0, pipesWidth, pipesHeight, 5, &pipesSprite, 0.05f, Colors::Magenta),
		borderOffset(borderOffset),
		wallsExpanded(walls.GetExpanded(float(borderOffset + pipesWidth), float(borderOffset + pipesWidth), 0.0f, float(borderOffset + pipesWidth))),
		wallsBackground(walls.GetExpanded(float(borderOffset + 1), float(borderOffset + 1), 0.0f, float(borderOffset + 1))),
		nBackgroundTilesX(wallsBackground.GetWidth() / 121),
		nBackgroundTilesY(wallsBackground.GetHeight() / 100),
		srcRectPieceOfBackground(0, nBackgroundTilesX % 121, 0, 100)
	{
	}

	void Draw(Graphics& gfx) const
	{
		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 6; y++)
			{
				gfx.DrawSprite(wallsBackground.left + x * 121, wallsBackground.top + y * 100, background, SpriteEffect::Chroma(Colors::Magenta));
			}
		}
		{
			const int x = 5;
			for (int y = 0; y < 6; y++)
			{
				gfx.DrawSprite(wallsBackground.left + x * 121, wallsBackground.top + y * 100, srcRectPieceOfBackground, background, SpriteEffect::Chroma(Colors::Magenta));
			}
		}
		////////////////////////////////////

		Vec2 posLeftWall = { wallsExpanded.left, wallsExpanded.bottom - pipesHeight / 2 };
		Vec2 posRightWall = { wallsExpanded.right - pipesWidth, wallsExpanded.bottom - pipesHeight / 2 };

		// left
		gfx.DrawSprite(posLeftWall.x, posLeftWall.y, srcRectHalfPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
		posLeftWall -= {0.0f, pipesHeight};
		gfx.DrawSprite(posLeftWall.x, posLeftWall.y, srcRectPipeExtra, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
		posLeftWall -= {0.0f, pipesHeight};

		// right
		gfx.DrawSprite(posRightWall.x, posRightWall.y, srcRectHalfPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
		posRightWall -= {0.0f, pipesHeight};
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
		posRightWall -= {0.0f, pipesHeight};

		for (int i = 0; i < 5; i++)
		{
			// left
			gfx.DrawSprite(posLeftWall.x, posLeftWall.y, srcRectPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			posLeftWall -= {0.0f, pipesHeight};
			gfx.DrawSprite(posLeftWall.x, posLeftWall.y, srcRectPipeExtra, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			posLeftWall -= {0.0f, pipesHeight};
			// right
			gfx.DrawSprite(posRightWall.x, posRightWall.y, srcRectPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			posRightWall -= {0.0f, pipesHeight};
			gfx.DrawSprite(posRightWall.x, posRightWall.y, srcRectPipeExtra, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });
			posRightWall -= {0.0f, pipesHeight};
		}

		// left
		posLeftWall.y = wallsExpanded.top + pipesWidth;
		gfx.DrawSprite(posLeftWall.x, posLeftWall.y, srcRectPieceOfPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });

		// right
		posRightWall.y = wallsExpanded.top + pipesWidth;
		gfx.DrawSprite(posRightWall.x, posRightWall.y, srcRectPieceOfPipe, pipesSprite, SpriteEffect::Chroma{ Colors::Magenta });

		////////////////////////////////////
		////////////////////////////////////

		// swap
		const int pipesWidth = this->pipesHeight;
		const int pipesHeight = this->pipesWidth;

		Vec2 posTopWallLeft = { wallsExpanded.left, wallsExpanded.top };
		Vec2 posTopWallRight = { wallsExpanded.right - pipesCornerWidth, wallsExpanded.top };

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

		////////////////////////////////////
	}
	void Update(float dt)
	{
		switch (doorState)
		{
		case DoorState::Oppening:
			pipeAnimationOppening.Update(dt);
			if (pipeAnimationOppening.GetFullAnimationCount() >= 1) {
				doorState = DoorState::Laser;
			}
			break;
		case DoorState::Laser:
			pipeAnimationLaser.Update(dt);
			break;
		}
	}

public:
	void SetDoorStateToOppening()
	{
		doorState = DoorState::Oppening;
	}
	void AnimationSceneReset()
	{
		doorState = DoorState::Closed;
		pipeAnimationOppening.Reset();
		pipeAnimationLaser.Reset();
	}

public:
	bool IsAnimationSceneEnd() const
	{
		return pipeAnimationOppening.GetFullAnimationCount();
	}

private:
	const int borderOffset;
	RectF wallsExpanded;
	RectF wallsBackground;
	int nBackgroundTilesX;
	int nBackgroundTilesY;
	RectI srcRectPieceOfBackground;
	DoorState doorState = DoorState::Closed;
	static constexpr int pipesHeight = 50;
	static constexpr int pipesWidth = 20;
	static constexpr int pipesCornerWidth = 20;

	// Left and Right
	static constexpr RectI srcRectPipe = { 0, 20, 0, 50 };
	static constexpr RectI srcRectHalfPipe{ 0, 20, 0, 25 };
	static constexpr RectI srcRectPieceOfPipe = { 0, 20, 0, (Graphics::ScreenHeight - 11 * pipesHeight - pipesHeight / 2 - pipesWidth) };
	static constexpr RectI srcRectPipeExtra = { 20, 40, 0, 50 };

	// Top
	static constexpr RectI srcRectTopPipeExtra = { 0, 50, 50, 70 };
	static constexpr RectI srcRectTopPipe = { 0, 50, 70, 90 };
	static constexpr RectI srcRectTopLeftCornerPipe = { 50, 70, 70, 90 };
	static constexpr RectI srcRectTopRightCornerPipe = { 70, 90, 70, 90 };
	static constexpr RectI srcRectTopPipeLong = { 90, 180, 70, 90 };
	Sprite pipesSprite;
	Sprite background;
	Animation pipeAnimationOppening;
	Animation pipeAnimationLaser;
	Animation topPipeAnimationOppening;
};