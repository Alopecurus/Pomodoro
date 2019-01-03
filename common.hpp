/*
* @file    common.h
* @brief   Common header file
* @auther  Alopecurus
*/
#ifndef POMODORO_COMMON_H_
#define POMODORO_COMMON_H_

#define ID_BUTTON  (300)

#include <windows.h>
#include <string>

typedef struct tagVector2 {
	int x;
	int y;
} vector2;

constexpr vector2 CLIENT_SIZE = {
	370,
	225
};

const COLORREF RED[2] = {
	RGB(255, 0, 0),
	RGB(255, 255, 255),
};
const COLORREF GREEN[2] = {
	RGB(0, 255, 72),
	RGB(0, 178, 51),
};

constexpr auto WHITE = RGB(255, 255, 255);

#endif // !POMODORO_COMMON_H_
