#include "ball_move.h"

#define PI 3.14159

// 마우스 방향으로 공 발사.
void shoot_ball(BALL &ball, POINT start_point, POINT mouse_point, double speed) 
{
	double gradient;	// 기울기.
	double degree;		// 각도.
	double x, y;

	x = (mouse_point.x - start_point.x);
	y = (mouse_point.y - start_point.y);

	gradient = y / x;

	if (y * x >= 0)	// 각도를 구하는 함수. 역함수들은 0~180사이값 만 반환한다. 이유는 0~360 사이에 기울기값을 만족하는 값이 2개 존재하기 때문.
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

// 각 벽의 기울기를 미리 구해준다.
void get_gradients(POINT point[], double gradients[], int size)	
{
	int i;
	double x, y;

	for (i = 0; i < size; i++)
	{
		if (i == (size - 1))	// 마지막 벽 포인터는 처음위치와 벽을 생성하기때문에 따로 지정.
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

// 새 게임을 위한 공과 상태정보 초기화.
void init_ball(BALL &ball, POINT start_point)
{
	ball.x = start_point.x;
	ball.y = start_point.y;

	ball.draw = false;
	ball.reflection = false;
	ball.collision_wall = 0;
}