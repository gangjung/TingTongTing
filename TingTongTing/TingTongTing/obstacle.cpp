#include "obstacle.h"

// ���ع��� ���⸦ ���.
void get_gradient_obstacle(OBSTACLE &obstacle)
{
	obstacle.gradient = (double)(obstacle.point[1].y - obstacle.point[0].y) / (obstacle.point[1].x - obstacle.point[0].x);
}