#pragma once
#include "way_search.h"
#include <stdlib.h>
#define DISTANCE_TO_STATE 400
#define DISTANCE_TO_WALK 150
#define RANGE_LITTLE 200
typedef enum {Attack, Patrol, Walk} State;

typedef struct
{
	State state;
	Point cur_cords;
	int distance;
}Enemy;


int distance_to_player(Point player_cord, Point enemy_cord);
Enemy* state_machine(Enemy* enemy, Point player_cord, int* grid, int* new_calc, int* ind);
Point* spawn_enemy(int* grid);