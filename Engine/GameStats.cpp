#include "GameStats.h"

GameStats::GameStats(const Font* font, const RectF& rect, int hp)
	:
	font(font),
	rect(rect),
	hp(hp),
	hpMax(hp)
{
}

void GameStats::Draw(Graphics& gfx)
{	
	gfx.DrawRect(rect, Colors::Black);
}

void GameStats::Update(float dt)
{
	time -= dt;
}

void GameStats::SetTime(float time)
{
	this->time = time;
}

void GameStats::HpReset()
{
	hp = hpMax;
}

void GameStats::HpSubtract()
{
	hp--;
}

int GameStats::GetHp() const
{
	return hp;
}

