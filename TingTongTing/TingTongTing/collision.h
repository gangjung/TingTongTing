#pragma once
#ifndef _COLLISION_
#define _COLLISION_

#include <windows.h>
#include "ball.h"

// ��� ���� �浹 Ȯ��.
void check_collision(POINT point[], double gradients[], int size, BALL &ball);

// ������ �浹 Ȯ��.
BOOL check_contact(POINT point1, POINT point2, BALL ball, double gradient);

// ������ �浹�� Ȯ���ϱ����� ���� �ε����� �����ȿ� �ִ��� Ȯ��.
BOOL check_range(POINT point1, POINT point2, BALL ball, double gradient);

#endif

