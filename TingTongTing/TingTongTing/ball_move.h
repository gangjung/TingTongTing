#pragma once
#ifndef _BALL_MOVE_
#define _BALL_MOVE_

#include <windows.h>
#include "ball.h"
#include <math.h>

void shoot_ball(BALL &ball, POINT start_point, POINT mouse_point, double speed);

// 각 벽의 기울기를 미리 구해준다.
void get_gradients(POINT point[], double gradients[], int size);

void init_ball(BALL &ball, POINT start_point);

#endif