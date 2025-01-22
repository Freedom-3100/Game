#pragma once
#include "way_search.h"
#include <stdlib.h>
#define DISTANCE_TO_STATE 400
#define DISTANCE_TO_WALK 150
#define RANGE_LITTLE 200
#define BULLET_RANGE 300
#define BULLET_SIZE 10
#define WALK_UPDATE_INTERVAL 5
typedef enum { Patrol, Attack,  Walk} State;
typedef enum {Yes, No} Damage;
typedef enum {Dead, Life} Check_Condition;

typedef struct
{
	Point cur_cords;
	Point end_cords;
	
}Bullet_Queue;

typedef struct
{
	State state;
	Point cur_cords;
	int distance;
	Check_Condition is_life;
	Bullet_Queue* queue;
}Enemy;




int distance_to_player(Point player_cord, Point enemy_cord);
Enemy* state_machine(Enemy* enemy, Point player_cord, int* grid, int* new_calc, int* ind, int* lenght);
Point spawn_enemy(int* grid);
Enemy* check_collision_bullet(Enemy* enemy, Point player_cord, int* lenght, Damage* damage);
Point change_cords_bullet(Point cur_cord, Point end_cords, int speed);