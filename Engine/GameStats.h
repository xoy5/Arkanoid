#pragma once
#include "Graphics.h"

#include "string"

#include "Font.h"
#include "Rect.h"
#include "Button.h"

class GameStats
{
public:
	GameStats(const Font* font, const Font* fontLg, const std::string& filenameRecords, const RectI& rect, int hp, int rounds);
	void Draw(Graphics& gfx) const;
	void DrawEndScreen(Graphics& gfx) const;
	void Update(float dt);

public:
	void NextRound();
	void AddPoints(int points);
	void ResumeTimer();
	void PauseTimer();
public:
	void HpReset();
	void HpSubtract();
	int GetHp() const;
	int GetRound() const;


private:
	const Font* font;
	const Font* fontLg;
	Button buttonBackToMenu;
	bool endScreen = false;
	const int charWidth;
	const int charHeight;
	const int fullChars;
	const int restPixels;
	const RectI rect;
	const int hpMax;
	int hp;
	int recordScore;
	bool timerWork = true;
	int score = 0;
	int round = 1;
	const int nRounds;
	float time = 0.0f;
};