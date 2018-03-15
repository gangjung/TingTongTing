#include "check_collision_point.h"
#include <math.h>
#include "reflection.h"

// �� �߿��� Ƣ��� ���� ������ ������ �Ǻ��ؾ��ϱ� ������ �浹�� Ȯ�����ش�.
void check_collision_point(POINT point, BALL &ball)
{
	double x;
	double y;
	double distance;
	double gradient;

	x = point.x - ball.x;
	y = point.y - ball.y;

	distance = sqrt((y*y) + (x*x));

	if (distance <= ball.size)	// ���� �浹����.
	{
		gradient = (-1)*(x / y);

		reflection(point, point, ball, gradient, 3);
		Sound_->generate_system("", SOUND_EFFECT_PLAY);
		Sound_->generate_system("sound\\Jump.mp3", SOUND_EFFECT_PLAY);
	}
}