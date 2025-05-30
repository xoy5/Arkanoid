#pragma once
#include "string"

#include "Graphics.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Font.h"
#include "Rect.h"
#include "Button.h"
#include "TextBox.h"

class GameStats
{
public:
	GameStats(const Font* font, const Font* fontLg, const std::string& filenameRecords, const RectI& rect, int hp, int rounds);

	void DrawStats(Graphics& gfx) const;
	void DrawEndScreen(Graphics& gfx) const;
	void DrawRanking(Graphics& gfx) const;

	void Update(float dt);
	void ProcessMouse(const Mouse::Event& event);
	void ProcessTextBox(char character);
	void Reset();
	void EndScoreCount();

public:
	void AddPointsEndRound();
	bool IsButtonClicked() const;
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
	TextBox textBoxName;
	
	const RectI rect;
	bool gameEnd = false;

	const int hpMax;
	int hp;

	int recordScore;
	int score = 0;

	float time = 0.0f;
	bool timerWork = true;

	static constexpr int pointsPerRound = 10000;
	static constexpr int pointsPerHp = 4000;
	static constexpr int pointsPerSecond = 100;
	
	int tempPlaceOfScore = 0;
	int tempPlaceOfScoreMinus = 0;
	float tempFullScore = 0.0f;
	bool animationAddingPoints = false;
	static constexpr float timeAddingPoints = 2.0f;
	static constexpr float deltaAdd = 1.0f / timeAddingPoints;
	float timeAddingPointsCount = 0.0f;


	int round = 1;
	const int nRounds;

	const int charWidth;
	const int charHeight;
	const int fullChars;
	const int restPixels;
};