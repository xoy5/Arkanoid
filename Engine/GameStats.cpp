#include "GameStats.h"
#include <fstream>
#include <assert.h>

GameStats::GameStats(const Font* font, const std::string& filenameRecords, const RectI& rect, int hp)
	:
	font(font),
	charWidth(font->GetWidthChar()),
	charHeight(font->GetHeightChar()),
	fullChars(rect.GetWidth() / charWidth - 1),
	restPixels(rect.GetWidth() % charWidth + charWidth),
	rect(rect),
	hp(hp),
	hpMax(hp)
{
	std::ifstream file(filenameRecords, std::ios_base::in);
	assert(file);
	std::string line;
	std::getline(file, line);
	if (line == "") {
		recordScore = 0;
	}
	else {
		recordScore = std::atoi(line.c_str());
	}
	file.close();
}

void GameStats::Draw(Graphics& gfx) const
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

void GameStats::NextRound()
{
	round++;
	
}

void GameStats::AddPoints(int points)
{
	score += points;
}

void GameStats::ResumeTimer()
{
	timerWork = true;
}

void GameStats::PauseTimer()
{
	timerWork = false;
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

int GameStats::GetRound() const
{
	return round;
}

