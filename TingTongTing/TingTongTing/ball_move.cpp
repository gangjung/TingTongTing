#include "ball_move.h"

#define PI 3.14159

// ���콺 �������� �� �߻�.
void shoot_ball(BALL &ball, POINT start_point, POINT mouse_point, double speed) 
{
	double gradient;	// ����.
	double degree;		// ����.
	double x, y;

	x = (mouse_point.x - start_point.x);
	y = (mouse_point.y - start_point.y);

	gradient = y / x;

	if (y * x >= 0)	// ������ ���ϴ� �Լ�. ���Լ����� 0~180���̰� �� ��ȯ�Ѵ�. ������ 0~360 ���̿� ���Ⱚ�� �����ϴ� ���� 2�� �����ϱ� ����.
	{
		if (x >= 0)
			ball.radian = atan(gradient);
		else
			ball.radian = atan(gradient) + PI;
	}
	else
	{
		if (x >= 0)
			ball.radian = atan(gradient);
		else
			ball.radian = atan(gradient) + PI;
	}
}

// �� ���� ���⸦ �̸� �����ش�.
void get_gradients(POINT point[], double gradients[], int size)	
{
	int i;
	double x, y;

	for (i = 0; i < size; i++)
	{
		if (i == (size - 1))	// ������ �� �����ʹ� ó����ġ�� ���� �����ϱ⶧���� ���� ����.
		{
			x = (point[0].x - point[i].x);
			y = (point[0].y - point[i].y);

			if ((x == 0) || (y == 0))
				gradients[i] = 0;
			else
				gradients[i] = y / x;
		}
		else
		{
			x = (point[i + 1].x - point[i].x);
			y = (point[i + 1].y - point[i].y);

			if ((x == 0) || (y == 0))
				gradients[i] = 0;
			else
				gradients[i] = y / x;
		}
	}
}

// �� ������ ���� ���� �������� �ʱ�ȭ.
void init_ball(BALL &ball, POINT start_point)
{
	ball.x = start_point.x;
	ball.y = start_point.y;

	ball.draw = false;
	ball.reflection = false;
	ball.collision_wall = 0;
}