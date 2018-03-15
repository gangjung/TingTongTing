#pragma once
#ifndef _BALL_
#define _BALL_

#include<windows.h>

// ��(����ü)������ ����.
struct BALL {
	double x;
	double y;
	double speed = 5; // �������� ���ؼ� �ʿ���� ����������.
	double radian = 0;
	bool draw = false;
	bool reflection = false;	// �浹�������� Ȯ��.
	bool reflection_obstacle = false;
	int collision_wall = 0;	// �浹�� ���� ������� Ȯ��
	int size = 7;	// ��ũ��
	int collisionCount = 0;	// �ε��� Ƚ��.
};

#endif
