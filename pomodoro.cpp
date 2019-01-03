/*
* @file    pomodoro.cpp
* @brief   ImplementPomodoro
* @auther  Alopecurus
*/

#include "pomodoro.hpp"

namespace pomodoro {

static constexpr int ID_TIMER = 1;

enum StateList
{
	STATE_POMODORO,
	STATE_REST,
	STATE_LONGREST,
	STATE_MAX,
};

static int timeList[STATE_MAX] = {
	60,
	60,
	60,
};

static bool enablePause;
static int remainingTime;
static int currrentScene;
static int countPomodoro;


void init(HWND hWnd, HINSTANCE hInstance)
{
	char Path[MAX_PATH];

	enablePause = true;
	remainingTime = 0;
	currrentScene = STATE_REST;
	countPomodoro = 0;

	GetCurrentDirectory(MAX_PATH, Path);
	wsprintf(Path, TEXT("%s\\%s"), Path, TEXT("pomodoro.ini"));

	timeList[0] *= GetPrivateProfileInt(TEXT("Setting"), TEXT("Pomodoro"), 25, Path);
	timeList[1] *= GetPrivateProfileInt(TEXT("Setting"), TEXT("Rest"), 5, Path);
	timeList[2] *= GetPrivateProfileInt(TEXT("Setting"), TEXT("LongRest"), 30, Path);

	// •b’PˆÊ‚ÅŒv‘ª
	SetTimer(hWnd, ID_TIMER, 1000, NULL);
	stateManager();
}

void fin(HWND hWnd)
{
	KillTimer(hWnd, ID_TIMER);
}

void update()
{
	if (!enablePause)
		remainingTime--;

	if (-1 > remainingTime)
		stateManager();
}

void stateManager()
{
	if (currrentScene == STATE_POMODORO && (countPomodoro % 4) == 0 && countPomodoro != 0)
	{
		currrentScene = STATE_LONGREST;
	}
	else if (currrentScene == STATE_POMODORO)
	{
		currrentScene = STATE_REST;
	}
	else
	{
		countPomodoro++;
		currrentScene = STATE_POMODORO;

	}

	if (countPomodoro != 1)
	{
		Beep(440, 1000);
	}

	remainingTime = timeList[currrentScene];
}

void pauseSwitch(HWND hWnd, HINSTANCE hInstance)
{
	MessageBeep(MB_OK);

	enablePause = !enablePause;
}

int getCountPomodoro()
{
	return countPomodoro;
}

int getRemainingTime()
{
	if (remainingTime > 0)
	{
		return remainingTime;
	}
	return 0;
}

bool getEnablePause()
{
	return enablePause;
}

}

