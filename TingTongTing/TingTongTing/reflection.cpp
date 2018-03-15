#include "reflection.h"
#include <math.h>
#define PI 3.14159
#define WALL 1
#define OBSTACLE 2

void reflection(POINT point1, POINT point2, BALL &ball, double gradient, int object)
{
	if (gradient)
	{
		double k;
		double o;
		double enter_speed;

		enter_speed = ball.speed;

		k = atan2(gradient, 1);

		if (k > PI) // 기울기이기때문에 180도보다 클 수 없다.
			k -= PI;
		else if (k < 0)
			k += PI;


		ball.radian = 2 * k - ball.radian;

		if (object == WALL)	// 1이면 벽과의 충돌
			ball.reflection = true;
		else if (object == OBSTACLE)	// 2이면 방해물과의 충돌.
			ball.reflection_obstacle = true;
	}
	else
	{
		if (point2.x == point1.x)	// y축과 평행한 선. x속도 방향만 바꿔주면 된다.
		{
			ball.radian = PI - ball.radian;
			if (object == WALL)
				ball.reflection = true;
			else if (object == OBSTACLE)
				ball.reflection_obstacle = true;
		}
		else						// x축과 평행한 선. y속도 방향만 바꿔주면 된다.
		{
			ball.radian -= ball.radian * 2;
			if (object == WALL)
				ball.reflection = true;
			else if (object == OBSTACLE)
				ball.reflection_obstacle = true;
		}
	}
	ball.collisionCount++;	// 벽과 충돌해서 반사되어 나가면 충돌 횟수 증가.
}