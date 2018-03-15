#include "StageClear.h"
#include <math.h>

// 스태이지를 클리어했는지 확인.
bool checkClear(POINT end_point, BALL ball, int endSize) 
{
	double x;
	double y;
	double distance;

	x = end_point.x - ball.x;
	y = end_point.y - ball.y;

	distance = sqrt((y*y) + (x*x));

	if (distance <= endSize)	// 도착점과의 거리를 계산해서 도착점에 도달하면 클리어.
		return true;

	return false;
}