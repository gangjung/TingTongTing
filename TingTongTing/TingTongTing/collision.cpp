#include <math.h>
#include "collision.h"
#include "reflection.h"
#include "Sound.h"
#include "check_collision_point.h"

//���� ���� ������ �Դ��� Ȯ���ϴ� ��.
BOOL check_range(POINT point1, POINT point2, BALL ball, double gradient)
{
	/*
	���� ���� ������ �ִ��� Ȯ���ϱ����ؼ��� �� ���� �� ���� ������ ������ �����Ǵ� ���� ���̿� �־�� �Ѵ�.
	�� ���� ���̿� �������� ���� ��ġ�� �� ���������Ŀ� �־�� ���� ������ ������ ���̿� �ִ� ���̴�.
	*/

	if (gradient)// ���Ⱑ x���̳� y��� �������� ���� ��.
	{
		double c1, c2;
		double value1, value2;
		double check;

		c1 = point1.y + point1.x / gradient;	// C = y1 + ((1/a) * x1)
		c2 = point2.y + point2.x / gradient;

		value1 = ball.x / gradient + ball.y - c1;	// Y = (-(1/a)*X) + C.   ->   ((1/a)*X) + Y - C = 0 �� Ȱ��
		value2 = ball.x / gradient + ball.y - c2;

		check = value1 * value2;

		if (check <= 0)
			return TRUE;
		else
			return FALSE;
	}
	else	// ���Ⱑ x���̳� y��� ������ ��. �����ϴ� ������ ���⸦ ���� ��, y��� ������ ��Ȳ�� ���� �� ���� ����.
	{
		// 0�̸� ���Ⱑ x��� ����, 1�̸� ���Ⱑ y��� ����.
		long check_gradient;

		long long_point;
		long short_point;

		check_gradient = point1.x - point2.x;

		// 0�̸� x��� ����, 1�̸� y��� ����
		if (check_gradient)
			check_gradient = 0;
		else
			check_gradient = 1;

		// x��� �������� y��� �������� Ȯ���� ��, �װͿ� �´� ���� ó��.
		switch (check_gradient)
		{
		case 0:
			// x�� ���̰��� ���ϱ����ؼ� ū ���� �������� �̸� �����صд�. �������� ������ ũ�ٰ� �� ���� ������ ������ ������ ũ�ٴ� ������ ���⶧��.
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

			// ���̿� �ִ��� Ȯ��.
			if ((short_point <= ball.x) && (ball.x <= long_point))
				return TRUE;
			else
				return FALSE;

			break;

		case 1:
			// x�� ���̰��� ���ϱ����ؼ� ū ���� �������� �̸� �����صд�. �������� ������ ũ�ٰ� �� ���� ������ ������ ������ ũ�ٴ� ������ ���⶧��.
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

			// ���̿� �ִ��� Ȯ��.
			if ((short_point <= ball.y) && (ball.y <= long_point))
				return TRUE;
			else
				return FALSE;

			break;
		}
	}
	return FALSE;
}

// ���� �ε������� Ȯ��.
BOOL check_contact(POINT point1, POINT point2, BALL ball, double gradient)
{
	/*
	�������� �������� ���Ѵ��� ������ ���� �Ÿ� ������ �̿��ؼ� �����ߴ��� ���ߴ��� Ȯ���Ѵ�.
	- aX + bY + c = 0. ( a = K(����), b = -1, c = y1 - K*x1)
	- d = |(ax1 + by1 + c)| / sqrt(a^2 + b^2).

	=> d = |(K*x1 + (-1)*y1 + c)| / sqrt(K^2 + 1).
	*/

	// �Ÿ�.
	double distance;

	// ����.
	double child;
	// �и�.
	double mother;

	if (gradient)
	{
		child = (gradient * ball.x) + ((-1) * ball.y) + (point1.y - (gradient * point1.x));

		mother = sqrt(gradient*gradient + 1);

		distance = child / mother;
	}
	else if (point1.x == point2.x)	// y��� ����
	{
		distance = ball.x - point1.x;
	}
	else	// x��� ����
	{
		distance = ball.y - point1.y;
	}

	// �Ÿ��� ��Ÿ���� ���̹Ƿ� ���밪�̿����Ѵ�.
	if (distance < 0)
		distance *= -1;

	if (distance <= ball.size)
		return TRUE;
	return FALSE;
}

// ��� ���� �浹 Ȯ��.
void check_collision(POINT point[], double gradients[], int size, BALL &ball)
{
	int i;

	for (i = 0; i < size - 1; i++)
	{
		if (ball.reflection)	// ���� �浹�����̸� �ٸ� ������ �浹�� ó�������� �ʴ´�.
		{
			if (i == ball.collision_wall)
			{
				// ������ �ε��ƴ� ������ ������� Ȯ��.
				// ������� �ٽ� �ٸ� ����� �浹 �����ϰ� �����.
				if (!check_contact(point[i], point[i + 1], ball, gradients[i]))
					ball.reflection = false;
				else
					break;
			}
			continue;
		}

		if (check_range(point[i], point[i + 1], ball, gradients[i]))	// ������ �浹 ���� ����.
		{
			if (check_contact(point[i], point[i + 1], ball, gradients[i]))	// �浹����.
			{
				reflection(point[i], point[i + 1], ball, gradients[i], 1);	// �ݻ�ó��.
				ball.collision_wall = i;
				Sound_->generate_system("", SOUND_EFFECT_PLAY);
				Sound_->generate_system("sound\\Jump.mp3", SOUND_EFFECT_PLAY);
			}
		}
		check_collision_point(point[i], ball);	// ������� �浹 Ȯ��.
	}

	// ������ ���� ó������ ���� ����� ������ ���� ����.
	if (ball.reflection)
	{
		if ((size - 1) == ball.collision_wall)
		{
			if (!check_contact(point[size - 1], point[0], ball, gradients[size - 1]))
				ball.reflection = false;
		}
		return;
	}

	if (check_range(point[size - 1], point[0], ball, gradients[size - 1]))	// ���� ���ִ� ������ ������ �����ʹ� ó�� �����Ϳ� ����Ǿ��ֱ� ����.
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