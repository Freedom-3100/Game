#include "enemy.h"
#include <stdio.h>

int distance_to_player(Point player_cord, Point enemy_cord)
{
	return abs(player_cord.x - enemy_cord.x) + abs(player_cord.y - enemy_cord.y);
}




static Enemy* check_state(Enemy* enemy, Point player_cord)
{

	if (enemy->distance > DISTANCE_TO_STATE)
	{
		enemy->state = Patrol;
	}
	else if (enemy->distance <= DISTANCE_TO_STATE && enemy->distance > DISTANCE_TO_WALK)
	{
		enemy->state = Walk;
	}
    else if (enemy->distance <= DISTANCE_TO_WALK && enemy->distance > 0)
	{
		enemy->state = Attack;
	}
    else
    {
        enemy->state = Patrol;
    }

	return enemy;
}

static Point* walk_to_point(int* grid, Point cord_enemy, Point cord_player) {

    int lenght;
    Point* path = BFS(grid, cord_enemy, cord_player, &lenght);
    return path;
}

Point change_cords_bullet(Point cur_cord, Point end_cords, int speed )

{
    double xn = (cur_cord.x - end_cords.x) / (sqrt((cur_cord.x - end_cords.x) * (cur_cord.x - end_cords.x) + (cur_cord.y - end_cords.y) * (cur_cord.y - end_cords.y))) * speed;
    double yn = (cur_cord.y - end_cords.y) / (sqrt((cur_cord.x - end_cords.x) * (cur_cord.x - end_cords.x) + (cur_cord.y - end_cords.y) * (cur_cord.y - end_cords.y))) * speed;
    cur_cord.x -= (int)xn;
    cur_cord.y -= (int)yn;
    return cur_cord;
}

static Enemy* add_bullet(Enemy* enemy, Point player_cord,  int * lenght)
{
    
    enemy->queue[*lenght] = (Bullet_Queue){ {enemy->cur_cords.x + ENEMY_SIZE, enemy->cur_cords.y}, {player_cord.x + 10, player_cord.y + 10} };

    *lenght += 1;
    enemy->is_life = Life;
    return enemy;
}

static void del_bullet(Bullet_Queue * queue, int* n,int ind) {

    int i = 0;
    while (i != ind) i++;


    for (int j = i; j < *n - 1; j++) {
        queue[i] = queue[i + 1];
    }

    (*n)--;
}


Enemy* check_collision_bullet(Enemy* enemy, Point player_cord, int* lenght, Damage * damage)
{
    Point real_cord = { player_cord.x + 10, player_cord.y + 10 };
    for (int i = 0; i < *lenght ; i++)
    {
        
        if (distance_to_player(enemy->queue[i].cur_cords, enemy->queue[i].end_cords) < 5 || enemy->queue[i].cur_cords.x < 0 || enemy->queue[i].cur_cords.y < 0 || enemy->queue[i].cur_cords.x + BULLET_SIZE > WINDOW_WIDTH
            || enemy->queue[i].cur_cords.y + BULLET_SIZE > WINDOW_HIGH)
        {
            del_bullet(enemy->queue, lenght, i);
            enemy->is_life = Dead;
        }
        else if (distance_to_player(enemy->queue[i].cur_cords, real_cord) < 10)
        {

            *damage = Yes;
            del_bullet(enemy->queue, lenght, i);
            enemy->is_life = Dead;
        }
    }
    
}

Enemy* state_machine(Enemy* enemy, Point player_cord, int* grid, int* new_calc, int* ind, int* length) {
    enemy = check_state(enemy, player_cord);

    static Point* path = NULL;            // Статическая переменная для хранения пути
    static Point last_player_cord = { 0 };  // Последние известные координаты игрока
    static int path_calculation_counter = 0; // Счётчик для редкого вызова walk_to_point

    if (enemy->state == Walk) {
        int new_calculation = distance_to_player(player_cord, enemy->cur_cords);

        // Проверяем, изменилось ли положение игрока или пора ли пересчитывать путь
        if (path == NULL || (player_cord.x != last_player_cord.x ||  player_cord.y != last_player_cord.y) ||
            path_calculation_counter >= WALK_UPDATE_INTERVAL) {

                if (path != NULL) {
                    free(path);  // Освобождаем предыдущий путь
                }

                path = walk_to_point(grid, enemy->cur_cords, player_cord);  // Генерируем новый путь
                last_player_cord = player_cord;  // Обновляем координаты игрока
                path_calculation_counter = 0;    // Сбрасываем счётчик
        }

        // Увеличиваем счётчик для редких обновлений пути
        path_calculation_counter++;

        // Уточняем необходимость обновления пути в зависимости от расстояния
        if (new_calculation <= RANGE_LITTLE) {
            if (path != NULL) {
                free(path);
            }
            path = walk_to_point(grid, enemy->cur_cords, player_cord);
        }
        else if (new_calculation <= *new_calc) {
            if (path != NULL) {
                free(path);
            }
            path = walk_to_point(grid, enemy->cur_cords, player_cord);
            *new_calc -= 40;
        }

        // Перемещение врага по рассчитанному пути
        if (path != NULL && *ind < WINDOW_HIGH * WINDOW_WIDTH) {
            enemy->cur_cords = path[*ind];
            (*ind)++;
            enemy->distance = distance_to_player(enemy->cur_cords, player_cord);

            // Проверяем, если состояние врага изменилось
            if (check_state(enemy, player_cord)->state != Walk) {
                free(path);
                path = NULL;  // Обнуляем указатель
            }
        }
    }

    if (enemy->state == Attack) {
        if (enemy->is_life == Dead) {
            enemy = add_bullet(enemy, player_cord, length);
        }
    }

    enemy->distance = distance_to_player(enemy->cur_cords, player_cord);
    return enemy;
}


static int is_area_free(int* grid, int start_row, int start_col) {
    for (int i = 0; i < ENEMY_SIZE; i++) {
        for (int j = 0; j < ENEMY_SIZE; j++) {
            if (start_row + i >= WINDOW_HIGH || start_col + j >= WINDOW_WIDTH ||
                grid[(start_row + i) * WINDOW_WIDTH + (start_col + j)] != 0) {
                return 0; 
            }
        }
    }
    return 1; 
}


Point spawn_enemy(int* grid) {
    Point spawn_point = { -1, -1 }; // Инициализация недопустимыми координатами
    for (int row = 0; row <= WINDOW_HIGH - ENEMY_SIZE; row++) {
        for (int col = 0; col <= WINDOW_WIDTH - ENEMY_SIZE; col++) {
            if (is_area_free(grid, row, col)) {
                spawn_point.x = col;
                spawn_point.y = row;
                return spawn_point;
            }
        }
    }
    return spawn_point; // Возвращаем недопустимый Point, если не найдено место
}
