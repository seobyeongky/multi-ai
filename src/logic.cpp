#include "logic.h"

const int point[3][3] = {{0, -1, 0}, {0, 0, -1}, {-1, 0, 0}};

int GetPoint(hand_t my_hand, hand_t enemy_hand)
{
	return point[my_hand][enemy_hand];
}