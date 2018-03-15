#include "check_collision_point.h"
#include <math.h>
#include "reflection.h"

// 벽 중에서 튀어나온 곳이 있으면 점으로 판별해야하기 때문에 충돌을 확인해준다.
void check_collision_point(POINT point, BALL &ball)
{
	double x;
	double y;
	double distance;
	double gradient;

	x = point.x - ball.x;
	y = point.y - ball.y;

	distance = sqrt((y*y) + (x*x));

	if (distance <= ball.size)	// 점과 충돌상태.
	{
		gradient = (-1)*(x / y);

		reflection(point, point, ball, gradient, 3);
		Sound_->generate_system("", SOUND_EFFECT_PLAY);
		Sound_->generate_system("sound\\Jump.mp3", SOUND_EFFECT_PLAY);
	}
}