#include "colision.h"

double distance(int cord_x_obj_1, int cord_y_obj_1, int cord_x_obj_2, int cord_y_obj_2)
{
	return(sqrt((cord_x_obj_1 - cord_x_obj_2) * (cord_x_obj_1 - cord_x_obj_2) + (cord_y_obj_1 - cord_y_obj_2) * (cord_y_obj_1 - cord_y_obj_2)));
}

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