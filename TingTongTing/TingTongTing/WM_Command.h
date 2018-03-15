#pragma once
#ifndef _WM_COMMAND_
#define _WM_COMMAND_

#include <Windows.h>
#include "ball.h"
#include "obstacle.h"

void WM_Command(HWND hwnd, WPARAM wParam, BALL &ball, OBSTACLE &obstacle, POINT start_point, HINSTANCE hInstance, int &stage, int &spendTime, bool &obstacleReset);

#endif
