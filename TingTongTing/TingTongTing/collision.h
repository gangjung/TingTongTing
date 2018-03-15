#pragma once
#ifndef _COLLISION_
#define _COLLISION_

#include <windows.h>
#include "ball.h"

// 모든 벽과 충돌 확인.
void check_collision(POINT point[], double gradients[], int size, BALL &ball);

// 벽과의 충돌 확인.
BOOL check_contact(POINT point1, POINT point2, BALL ball, double gradient);

// 벽과의 충돌을 확인하기위해 벽과 부딧히는 범위안에 있는지 확인.
BOOL check_range(POINT point1, POINT point2, BALL ball, double gradient);

#endif

