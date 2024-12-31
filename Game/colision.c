#include "colision.h"


bool check_colision(int cord_x, int cord_y,  int * matrix_room)
{
	if (matrix_room[cord_y * WINDOW_WIDTH + (cord_x)] == 1 || matrix_room[(cord_y + 35) * WINDOW_WIDTH + (cord_x + 30)] == 1 || matrix_room[cord_y * WINDOW_WIDTH + cord_x + 30] == 1 || matrix_room[(cord_y + 35) * WINDOW_WIDTH + cord_x] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}