#include "StageClear.h"
#include <math.h>

// ���������� Ŭ�����ߴ��� Ȯ��.
bool checkClear(POINT end_point, BALL ball, int endSize) 
{
	double x;
	double y;
	double distance;

	x = end_point.x - ball.x;
	y = end_point.y - ball.y;

	distance = sqrt((y*y) + (x*x));

	if (distance <= endSize)	// ���������� �Ÿ��� ����ؼ� �������� �����ϸ� Ŭ����.
		return true;

	return false;
}