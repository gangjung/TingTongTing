#pragma once
#ifndef _OBSTACLE_
#define _OBSTACLE_

#include <Windows.h>

struct OBSTACLE {
	POINT point[2];
	double gradient;

	bool draw = false;
};

void get_gradient_obstacle(OBSTACLE &obstacle);

#endif