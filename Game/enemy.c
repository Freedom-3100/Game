#include "enemy.h"

int distance_to_player(Point player_cord, Point enemy_cord)
{
	return abs(player_cord.x - enemy_cord.x) + abs(player_cord.y - enemy_cord.y);
}

static Enemy* create_enemy(Point end, Point start)
{
	Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
	enemy->distance = distance_to_player(start, end);
    enemy->cur_cords = start;
	enemy->state = Patrol;
    enemy->direction = Down;
	return enemy;
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
	else
	{
		enemy->state = Attack;
	}
	return enemy;
}

static Point* walk_to_point(int* grid, Point cord_enemy, Point cord_player) {
    // Вызываем BFS с одномерным массивом grid
    int lenght;
    Point* path = BFS(grid, cord_enemy, cord_player, &lenght);
    return path;
}




Enemy* state_machine(Enemy* enemy, Point player_cord, int* grid, int* new_calc, int* ind) {
    enemy = check_state(enemy, player_cord);
    
    if (enemy->state == Walk) {
        int new_calculation = distance_to_player(player_cord, enemy->cur_cords);
        
        // Получаем путь к игроку
        Point* path = walk_to_point(grid, enemy->cur_cords, player_cord);
        if (new_calculation <= RANGE_LITTLE) {
            path = walk_to_point(grid, enemy->cur_cords, player_cord);
        }
        else if (new_calculation <= new_calc) {
            
            path = walk_to_point(grid, enemy->cur_cords, player_cord);
            new_calc -= 40;
        }

        // Обновляем текущее положение врага
        if (path != NULL && *ind < WINDOW_HIGH * WINDOW_WIDTH) {
            enemy->cur_cords = path[*ind];
            *ind += 1;
            enemy->distance = distance_to_player(enemy->cur_cords, player_cord);
            if (check_state(enemy, player_cord)->state != Walk)
            {
                free(path);
            }
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
                return 0; // Область занята или выходит за пределы
            }
        }
    }
    return 1; // Область свободна
}

// Функция для поиска свободной области и возврата координат
Point* spawn_enemy(int* grid) {
    for (int row = 0; row <= WINDOW_HIGH - ENEMY_SIZE; row++) {
        for (int col = 0; col <= WINDOW_WIDTH - ENEMY_SIZE; col++) {
            if (is_area_free(grid, row, col)) {
                Point* spawn_point = malloc(sizeof(Point));
                if (spawn_point != NULL) {
                    spawn_point->x = col ;
                    spawn_point->y = row ;
                    return spawn_point; // Возвращаем координаты верхнего левого угла
                }
            }
        }
    }
    return NULL; // Если не нашли свободную область
}
// Почистся кажется надо