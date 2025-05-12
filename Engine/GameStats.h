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
	bool ProcessButton(const Mouse::Event& event);
	void Reset();

public:
	void NextRound();
	void AddPoints(int points);

	void ResumeTimer();
	void PauseTimer();

	void GameEndReset();
	void HpReset();
	void HpSubtract();

public:
	bool IsGameEnd() const;
	int GetHp() const;
	int GetRound() const;


private:
	const Font* font;
	const Font* fontLg;
	Button buttonBackToMenu;
	const RectI rect;
	bool gameEnd = false;

	const int hpMax;
	int hp;

	int recordScore;
	int score = 0;

	float time = 0.0f;
	bool timerWork = true;

	int round = 5;
	const int nRounds;

	const int charWidth;
	const int charHeight;
	const int fullChars;
	const int restPixels;
};