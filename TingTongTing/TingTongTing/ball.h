#pragma once
#ifndef _BALL_
#define _BALL_

#include<windows.h>

// 공(투사체)에관한 정보.
struct BALL {
	double x;
	double y;
	double speed = 5; // 각도값을 더해서 필요없게 만들어버리자.
	double radian = 0;
	bool draw = false;
	bool reflection = false;	// 충돌상태인지 확인.
	bool reflection_obstacle = false;
	int collision_wall = 0;	// 충돌한 벽이 어디인지 확인
	int size = 7;	// 공크기
	int collisionCount = 0;	// 부딪힌 횟수.
};

#endif
