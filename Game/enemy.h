#pragma once
#include "way_search.h"
#include <stdlib.h>
#define DISTANCE_TO_STATE 400
#define DISTANCE_TO_WALK 150
#define RANGE_LITTLE 200
#define BULLET_RANGE 300

typedef enum {Attack, Patrol, Walk} State;
typedef enum {Left, Right, Up, Down}Direction;
typedef enum {Dead, Life}Check_Condition;


typedef struct
{
	State state;
	Direction direction;
	Point cur_cords;
	int distance;
}Enemy;

typedef struct
{
	Check_Condition condition;
	Point cur_cords;
	Point end_cords;
	int speed;
	Direction direction;
}Bullet_Queue;


int distance_to_player(Point player_cord, Point enemy_cord);
Enemy* state_machine(Enemy* enemy, Point player_cord, int* grid, int* new_calc, int* ind);
Point* spawn_enemy(int* grid);