/*
* @file    pomodoro.h
* @brief   Implement the Pomodoro namespace
* @auther  Alopecurus
*/
#ifndef POMODORO_POMODORO_H_
#define POMODORO_POMODORO_H_

#include "common.hpp"

namespace pomodoro
{

void init(HWND hWnd, HINSTANCE hInstance);
void fin(HWND hWnd);
void update();

void stateManager();
void pauseSwitch(HWND hWnd, HINSTANCE hInstance);
int getCountPomodoro();
int getRemainingTime();
bool getEnablePause();

}

#endif // !POMODORO_POMODORO_H_
