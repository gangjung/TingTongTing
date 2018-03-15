#include <math.h>
#include "collision.h"
#include "reflection.h"
#include "Sound.h"
#include "check_collision_point.h"

//공이 벽의 범위에 왔는지 확인하는 것.
BOOL check_range(POINT point1, POINT point2, BALL ball, double gradient)
{
	/*
	공이 벽의 범위에 있는지 확인하기위해서는 벽 양쪽 끝 점을 지나는 각각의 직교되는 직선 사이에 있어야 한다.
	그 직선 사이에 있으려면 공의 위치를 각 직선방정식에 넣어보고 값이 음수가 나오면 사이에 있는 것이다.
	*/

	if (gradient)// 기울기가 x축이나 y축과 평행하지 않을 때.
	{
		double c1, c2;
		double value1, value2;
		double check;

		c1 = point1.y + point1.x / gradient;	// C = y1 + ((1/a) * x1)
		c2 = point2.y + point2.x / gradient;

		value1 = ball.x / gradient + ball.y - c1;	// Y = (-(1/a)*X) + C.   ->   ((1/a)*X) + Y - C = 0 을 활용
		value2 = ball.x / gradient + ball.y - c2;

		check = value1 * value2;

		if (check <= 0)
			return TRUE;
		else
			return FALSE;
	}
	else	// 기울기가 x축이나 y축과 평행할 때. 구분하는 이유는 기울기를 구할 때, y축과 평행한 상황을 구할 수 없기 때문.
	{
		// 0이면 기울기가 x축과 평행, 1이면 기울기가 y축과 평행.
		long check_gradient;

		long long_point;
		long short_point;

		check_gradient = point1.x - point2.x;

		// 0이면 x축과 평행, 1이면 y축과 평행
		if (check_gradient)
			check_gradient = 0;
		else
			check_gradient = 1;

		// x축과 평행한지 y축과 평행한지 확인한 뒤, 그것에 맞는 연산 처리.
		switch (check_gradient)
		{
		case 0:
			// x축 사이값을 구하기위해서 큰 값과 작은값을 미리 구분해둔다. 포인터의 순서가 크다고 그 값이 이전의 포인터 값보다 크다는 보장이 없기때문.
			if (point1.x > point2.x)
			{
				long_point = point1.x;
				short_point = point2.x;
			}
			else
			{
				long_point = point2.x;
				short_point = point1.x;
			}

			// 사이에 있는지 확인.
			if ((short_point <= ball.x) && (ball.x <= long_point))
				return TRUE;
			else
				return FALSE;

			break;

		case 1:
			// x축 사이값을 구하기위해서 큰 값과 작은값을 미리 구분해둔다. 포인터의 순서가 크다고 그 값이 이전의 포인터 값보다 크다는 보장이 없기때문.
			if (point1.y > point2.y)
			{
				long_point = point1.y;
				short_point = point2.y;
			}
			else
			{
				long_point = point2.y;
				short_point = point1.y;
			}

			// 사이에 있는지 확인.
			if ((short_point <= ball.y) && (ball.y <= long_point))
				return TRUE;
			else
				return FALSE;

			break;
		}
	}
	return FALSE;
}

// 벽과 부딧혔는지 확인.
BOOL check_contact(POINT point1, POINT point2, BALL ball, double gradient)
{
	/*
	벽에관한 방정식을 구한다음 직선과 점의 거리 공식을 이용해서 접촉했는지 안했는지 확인한다.
	- aX + bY + c = 0. ( a = K(기울기), b = -1, c = y1 - K*x1)
	- d = |(ax1 + by1 + c)| / sqrt(a^2 + b^2).

	=> d = |(K*x1 + (-1)*y1 + c)| / sqrt(K^2 + 1).
	*/

	// 거리.
	double distance;

	// 분자.
	double child;
	// 분모.
	double mother;

	if (gradient)
	{
		child = (gradient * ball.x) + ((-1) * ball.y) + (point1.y - (gradient * point1.x));

		mother = sqrt(gradient*gradient + 1);

		distance = child / mother;
	}
	else if (point1.x == point2.x)	// y축과 평행
	{
		distance = ball.x - point1.x;
	}
	else	// x축과 평행
	{
		distance = ball.y - point1.y;
	}

	// 거리를 나타내는 값이므로 절대값이여야한다.
	if (distance < 0)
		distance *= -1;

	if (distance <= ball.size)
		return TRUE;
	return FALSE;
}

// 모든 벽과 충돌 확인.
void check_collision(POINT point[], double gradients[], int size, BALL &ball)
{
	int i;

	for (i = 0; i < size - 1; i++)
	{
		if (ball.reflection)	// 공이 충돌상태이면 다른 벽과의 충돌을 처리해주지 않는다.
		{
			if (i == ball.collision_wall)
			{
				// 이전에 부딧쳤던 벽에서 벗어났는지 확인.
				// 벗어났으면 다시 다른 벽들과 충돌 가능하게 만든다.
				if (!check_contact(point[i], point[i + 1], ball, gradients[i]))
					ball.reflection = false;
				else
					break;
			}
			continue;
		}

		if (check_range(point[i], point[i + 1], ball, gradients[i]))	// 벽과의 충돌 범위 지정.
		{
			if (check_contact(point[i], point[i + 1], ball, gradients[i]))	// 충돌감지.
			{
				reflection(point[i], point[i + 1], ball, gradients[i], 1);	// 반사처리.
				ball.collision_wall = i;
				Sound_->generate_system("", SOUND_EFFECT_PLAY);
				Sound_->generate_system("sound\\Jump.mp3", SOUND_EFFECT_PLAY);
			}
		}
		check_collision_point(point[i], ball);	// 점들과의 충돌 확인.
	}

	// 마지막 점은 처음점과 벽을 만들기 때문에 따로 설정.
	if (ball.reflection)
	{
		if ((size - 1) == ball.collision_wall)
		{
			if (!check_contact(point[size - 1], point[0], ball, gradients[size - 1]))
				ball.reflection = false;
		}
		return;
	}

	if (check_range(point[size - 1], point[0], ball, gradients[size - 1]))	// 따로 해주는 이유는 마지막 포인터는 처음 포인터와 연결되어있기 때문.
	{
		if (check_contact(point[size - 1], point[0], ball, gradients[size - 1]))
		{
			reflection(point[size - 1], point[0], ball, gradients[size - 1], 1);
			ball.collision_wall = size - 1;
			Sound_->generate_system("", SOUND_EFFECT_PLAY);
			Sound_->generate_system("sound\\Jump.mp3", SOUND_EFFECT_PLAY);
		}
	}
	check_collision_point(point[i], ball);
}