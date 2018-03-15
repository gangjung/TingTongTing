#include "obstacle.h"

// 방해물의 기울기를 계산.
void get_gradient_obstacle(OBSTACLE &obstacle)
{
	obstacle.gradient = (double)(obstacle.point[1].y - obstacle.point[0].y) / (obstacle.point[1].x - obstacle.point[0].x);
}