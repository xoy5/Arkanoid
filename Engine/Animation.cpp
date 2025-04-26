#include "Animation.h"
#include "SpriteEffect.h"

Animation::Animation( int x,int y,int width,int height,int count,
					  const Sprite* sprite,float holdTime,Color chroma )
	:
	sprite( sprite ),
	holdTime( holdTime ),
	chroma( chroma )
{
	for( int i = 0; i < count; i++ )
	{
		frames.emplace_back( x + i * width,x + (i + 1) * width,y,y + height );
	}
}

void Animation::Update( float dt )
{
	curFrameTime += dt;
	while( curFrameTime >= holdTime )
	{
		Advance();
		curFrameTime -= holdTime;
	}
}

int Animation::GetFullAnimationCount() const
{
	return fullAnimationCount;
}

void Animation::ResetFullAnimationCount()
{
	fullAnimationCount = 0;
}

void Animation::Reset()
{
	ResetFullAnimationCount();
	curFrameTime = 0.0f;
}

void Animation::Advance()
{
	if( ++iCurFrame >= frames.size() )
	{
		fullAnimationCount++;
		iCurFrame = 0;
	}
}
