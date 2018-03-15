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

		if (k > PI) // �����̱⶧���� 180������ Ŭ �� ����.
			k -= PI;
		else if (k < 0)
			k += PI;


		ball.radian = 2 * k - ball.radian;

		if (object == WALL)	// 1�̸� ������ �浹
			ball.reflection = true;
		else if (object == OBSTACLE)	// 2�̸� ���ع����� �浹.
			ball.reflection_obstacle = true;
	}
	else
	{
		if (point2.x == point1.x)	// y��� ������ ��. x�ӵ� ���⸸ �ٲ��ָ� �ȴ�.
		{
			ball.radian = PI - ball.radian;
			if (object == WALL)
				ball.reflection = true;
			else if (object == OBSTACLE)
				ball.reflection_obstacle = true;
		}
		else						// x��� ������ ��. y�ӵ� ���⸸ �ٲ��ָ� �ȴ�.
		{
			ball.radian -= ball.radian * 2;
			if (object == WALL)
				ball.reflection = true;
			else if (object == OBSTACLE)
				ball.reflection_obstacle = true;
		}
	}
	ball.collisionCount++;	// ���� �浹�ؼ� �ݻ�Ǿ� ������ �浹 Ƚ�� ����.
}