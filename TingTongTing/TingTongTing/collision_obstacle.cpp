#include "collision_obstacle.h"
#include "ball.h"
#include "collision.h"
#include "reflection.h"
#include "sound.h"
#include "check_collision_point.h"

// ���ع����� �浹 Ȯ��.
void check_collision_obstacle(OBSTACLE &obstacle, BALL &ball)
{
	// ������ �浹 Ȯ��.
	check_collision_point(obstacle.point[0], ball);
	check_collision_point(obstacle.point[1], ball);

	if (ball.reflection_obstacle)	// ���ع����� �浹 ó���� ������ �����ϱ� ���� ����.
	{
		if (!check_contact(obstacle.point[0], obstacle.point[1], ball, obstacle.gradient))
			ball.reflection_obstacle = false;
		else
			return;
	}
	if (check_range(obstacle.point[0], obstacle.point[1], ball, obstacle.gradient))	// �浹 Ȯ��.
	{
		if (check_contact(obstacle.point[0], obstacle.point[1], ball, obstacle.gradient))
		{
			reflection(obstacle.point[0], obstacle.point[1], ball, obstacle.gradient, 2);
			Sound_->generate_system("", SOUND_EFFECT_PLAY);
			Sound_->generate_system("sound\\Jump.mp3", SOUND_EFFECT_PLAY);
		}
	}
}