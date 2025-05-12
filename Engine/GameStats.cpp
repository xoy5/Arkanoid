#include "GameStats.h"
#include <fstream>
#include <assert.h>
#include "Record.h"
#include <iomanip>

GameStats::GameStats(const Font* font, const Font* fontLg, const std::string& filenameRecords, const RectI& rect, int hp, int rounds)
	:
	font(font),
	fontLg(fontLg),
	buttonBackToMenu(font, "BACK TO MENU", Graphics::GetScreenCenter() + Vei2{ 0, 250 }),
	textBoxName(font, Vei2{ 300, 300 }),
	charWidth(font->GetWidthChar()),
	charHeight(font->GetHeightChar()),
	fullChars(rect.GetWidth() / charWidth - 1),
	restPixels(rect.GetWidth() % charWidth + charWidth),
	rect(rect),
	hp(hp),
	hpMax(hp),
	nRounds(rounds)
{
	textBoxName.SetLimit(true, 10);
	buttonBackToMenu.SetPositionCenter(true);
	std::ifstream file("Files/Records.txt", std::ios_base::in);
	if (!file)
	{
		std::ofstream("Files/Records.txt", std::ios_base::out);
		recordScore = 0;
		return;
	}

	std::string line;
	std::getline(file, line);
	std::stringstream ss(line);
	std::string temp;
	std::string highScoreText;

	std::getline(ss, temp, ';');
	std::getline(ss, temp, ';');
	std::getline(ss, highScoreText, ';');

	recordScore = std::stoi(highScoreText);
	file.close();;
}

void GameStats::DrawStats(Graphics& gfx) const
{
	Vei2 posCenter = Vei2{ rect.left + (restPixels) / 2, rect.top + charHeight };

	font->DrawText(" HIGH\n  SCORE", posCenter, Colors::BlueViolet, gfx, -5);
	posCenter.y += charHeight * 2 - 5;
	std::string strRecordScore = std::to_string(recordScore);
	std::string spacesForRecordScore(fullChars - int(strRecordScore.size()), ' ');
	font->DrawText(spacesForRecordScore + strRecordScore, posCenter, Colors::White, gfx);

	posCenter.y += 50;
	font->DrawText(" CUR\n  SCORE", posCenter, Colors::Cyan, gfx, -5);
	posCenter.y += charHeight * 2 - 5;
	std::string strCurScore = std::to_string(score);
	std::string spacesForCurScore(fullChars - int(strCurScore.size()), ' ');
	font->DrawText(spacesForCurScore + strCurScore, posCenter, Colors::White, gfx);

	posCenter.y += 50;
	font->DrawText(" LIFES\n   LEFT", posCenter, Colors::Red, gfx, -5);
	posCenter.y += charHeight * 2 - 5;
	std::string strHp = std::to_string(hp);
	std::string spacesForHp(fullChars - int(strHp.size()), ' ');
	font->DrawText(spacesForHp + strHp, posCenter, Colors::White, gfx);

	posCenter.y += 50;
	font->DrawText(" ROUND", posCenter, Colors::Green, gfx);
	posCenter.y += charHeight - 5;
	std::string strRound = std::to_string(round);
	std::string spacesForRound(fullChars - int(strRound.size()), ' ');
	font->DrawText(spacesForRound + strRound, posCenter, Colors::White, gfx);

	posCenter.y += 50;
	font->DrawText(" PLAY\n  TIME", posCenter, Colors::RedOrange, gfx, -5);
	posCenter.y += charHeight * 2 - 5;
	int minutes = time / 60.0f;
	int seconds = std::fmod(time, 60.0f);
	std::string strMinutes = std::to_string(minutes);
	std::string zerosForMinutes;
	if (strMinutes.size() <= 2) {
		zerosForMinutes = std::string(2 - int(strMinutes.size()), '0');
	}
	std::string strSeconds = std::to_string(seconds);
	std::string zerosForSeconds(2 - int(strSeconds.size()), '0');
	font->DrawText((strMinutes.size() <= 2 ? "  " : " ") + zerosForMinutes + strMinutes + ':' + zerosForSeconds + strSeconds, posCenter, Colors::White, gfx);

}

void GameStats::DrawEndScreen(Graphics& gfx) const
{
	Vei2 screenCenter = Graphics::GetScreenCenter();

	const bool isGameOver = hp <= 0;
	const std::string gameEndText = isGameOver ? "GAME OVER" : "GAME ENDED";
	const Color endTextColor = isGameOver ? Colors::Red : Colors::Green;

	std::string scoreText = "YOUR SCORE: " + std::to_string(score);
	std::string infoText = isGameOver ? "Better luck next time!" : "Well played!";

	font->DrawTextCenter("Nickname:", textBoxName.GetRect().GetPos() + Vei2(0, textBoxName.GetRect().GetHeight() / 2) - Vei2{120, 0}, Colors::White, gfx);

	fontLg->DrawTextCenter(gameEndText, screenCenter - Vei2{ 0, 200 }, endTextColor, gfx);

	fontLg->DrawTextCenter(scoreText, screenCenter - Vei2{ 0, 120 }, Colors::White, gfx);

	font->DrawTextCenter(infoText, screenCenter - Vei2{ 0, 60 }, Colors::Gray, gfx);

	// Draw name input and back to menu button
	textBoxName.Draw(gfx);
	buttonBackToMenu.Draw(gfx);
}

void GameStats::DrawRanking(Graphics& gfx) const
{
	Vei2 screenCenter = Graphics::GetScreenCenter();
	Vei2 pos = screenCenter - Vei2{ 0, 250 };
	auto records = getRecords("Files/Records.txt", 10);

	fontLg->DrawTextCenter("RANKING", pos, Colors::White, gfx);
	pos.y += 50;

	for (int i = 0; i < records.size(); i++)
	{
		std::ostringstream oss;
		oss <<  std::right << std::setw(2) << records[i].place << ". "
			<< std::left << std::setw(10) << records[i].name << " "
			<< std::right << std::setw(6) << records[i].score;
		const std::string content = oss.str();
		if (i == 0)
		{
			fontLg->DrawTextCenter(content, pos, Colors::Gold, gfx);
		}
		else if (i == 1)
		{
			fontLg->DrawTextCenter(content, pos, Colors::LightSteelBlue, gfx);
		}
		else if (i == 2)
		{
			fontLg->DrawTextCenter(content, pos, Colors::Sienna, gfx);
		}
		else
		{
			fontLg->DrawTextCenter(content, pos, Colors::White, gfx);
		}
		pos.y += charHeight + 10 /*padding*/;

		buttonBackToMenu.Draw(gfx);
	}

}

void GameStats::Update(float dt)
{
	if (timerWork)
	{
		time += dt;
	}
	if (score > recordScore)
	{
		recordScore = score;
	}
}

void GameStats::ProcessMouse(const Mouse::Event& event)
{
	buttonBackToMenu.ProcessMouse(event);
	textBoxName.ProcessMouse(event);

	if (buttonBackToMenu.IsClicked())
	{
		updateRecords("Files/Records.txt", textBoxName.GetText(), score);
	}
}

void GameStats::ProcessTextBox(char character)
{
	textBoxName.Interact(character);
}

void GameStats::Reset()
{
	score = 0;
	round = 1;
	time = 0.0f;
	GameEndReset();
	HpReset();
}

bool GameStats::IsButtonClicked() const
{
	return buttonBackToMenu.IsClicked();
}

void GameStats::NextRound()
{
	round++;
	if (round > nRounds)
	{
		gameEnd = true;
	}
}

void GameStats::AddPoints(int points)
{
	if (score + points > 1'000'000)
	{
		score = 1'000'000;
	}
	else
	{
		score += points;
	}
}

void GameStats::ResumeTimer()
{
	timerWork = true;
}

void GameStats::PauseTimer()
{
	timerWork = false;
}

void GameStats::GameEndReset()
{
	gameEnd = false;
}

void GameStats::HpReset()
{
	hp = hpMax;
}

void GameStats::HpSubtract()
{
	hp--;
	if (hp <= 0) {
		gameEnd = true;
	}
}

bool GameStats::IsGameEnd() const
{
	return gameEnd;
}

int GameStats::GetHp() const
{
	return hp;
}

int GameStats::GetRound() const
{
	return round;
}

