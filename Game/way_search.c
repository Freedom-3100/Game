#include <stdio.h>
#include "way_search.h"
#include <assert.h>

bool check_colision_enemy(int cord_x, int cord_y, int* matrix_room)
{
    if (matrix_room[cord_y * WINDOW_WIDTH + (cord_x)] == 1 || matrix_room[(cord_y + 30) * WINDOW_WIDTH + (cord_x + 30)] == 1 || matrix_room[cord_y * WINDOW_WIDTH + cord_x + 30] == 1 || matrix_room[(cord_y + 30) * WINDOW_WIDTH + cord_x] == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static int is_free(int col, int row, int* grid, int** visited) {
    // Проверяем, находится ли текущая ячейка в пределах границ и свободна ли она
    return (col >= 0 && col < WINDOW_WIDTH && row >= 0 && row < WINDOW_HIGH && grid[row * WINDOW_WIDTH + col] == 0 && !check_colision_enemy(col,row,grid) && !visited[row][col]);
}

static Point* path_recovery(Point** prev, Point end, int* path_length) {
    // Восстанавливаем путь от конечной точки
    Point* path = (Point*)malloc(WINDOW_HIGH * WINDOW_WIDTH * sizeof(Point));
    *path_length = 0;

    for (Point cur_point = end;  cur_point.x != -1 && cur_point.y != -1; cur_point = prev[cur_point.y][cur_point.x]) {
        path[(*path_length)++] = cur_point;
    }

    // Переворачиваем путь для правильного порядка
    Point* result = (Point*)malloc(*path_length * sizeof(Point));
    for (int i = 0; i < *path_length; i++) {
        result[i] = path[*path_length - 1 - i];
    }
    free(path); // Освобождаем память временного массива
    return result;
}

Point* BFS(int* grid, Point start, Point end, int* pathLength) {
    // Выделяем память для массива visited
    int** visited = (int**)malloc(WINDOW_HIGH * sizeof(int*));
    for (int i = 0; i < WINDOW_HIGH; i++) {
        visited[i] = (int*)malloc(WINDOW_WIDTH * sizeof(int));
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            visited[i][j] = 0; // Инициализируем все ячейки как непосещенные
        }
    }

    // Выделяем память для массива prev
    Point** prev = (Point**)malloc(WINDOW_HIGH * sizeof(Point*));
    for (int i = 0; i < WINDOW_HIGH; i++) {
        prev[i] = (Point*)malloc(WINDOW_WIDTH * sizeof(Point));
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            prev[i][j] = (Point){ -1, -1 }; // Инициализируем предшественников как недоступные
        }
    }

    // Направления движения (вверх, вниз, влево, вправо)
    int row_dir[] = { -1, 1, 0, 0 }; // Изменения по y
    int col_dir[] = { 0, 0, -1, 1 }; // Изменения по x

    // Создаем очередь для BFS
    Node* queue = (Node*)malloc(WINDOW_HIGH * WINDOW_WIDTH * sizeof(Node));
    int front = 0, backside = 0;

    // Добавляем начальную точку в очередь
    queue[backside++] = (Node){ start, 0, {-1, -1} };
    visited[start.y][start.x] = 1;

    while (front < backside) {
        Node current = queue[front++];

        // Если достигли конечной точки, восстанавливаем путь
        if (current.pt.x == end.x && current.pt.y == end.y) {
            Point* path = path_recovery(prev, end, pathLength);
            free(queue); // Освобождаем память очереди

            // Освобождаем память для visited и prev
            for (int i = 0; i < WINDOW_HIGH; i++) {
                free(visited[i]);
                free(prev[i]);
            }
            free(visited);
            free(prev);

            return path;
        }

        // Проверяем все возможные направления движения
        for (int i = 0; i < 4; i++) {
            int new_row = current.pt.y + row_dir[i];
            int new_col = current.pt.x + col_dir[i];

            if (is_free(new_col, new_row, grid, visited)) {
                visited[new_row][new_col] = 1; // Помечаем как посещенную
                queue[backside++] = (Node){ {new_col, new_row}, current.dist + 1, current.pt }; // Добавляем в очередь
                prev[new_row][new_col] = current.pt; // Запоминаем предшественника
            }
        }
    }

    // Если путь не найден
    free(queue); // Освобождаем память очереди


    // Освобождаем память для visited и prev
    for (int i = 0; i < WINDOW_HIGH; i++) {
        free(visited[i]);
        free(prev[i]);
    }
    free(visited);
    free(prev);

    return NULL;
}

//// Тестовые функции
//void test_BFS() {
//    // Пример 1: Путь существует
//    int grid1[5][5] = {
//        {0, 0, 0, 0, 0},
//        {0, 1, 1, 1, 0},
//        {0, 0, 0, 1, 0},
//        {0, 1, 0, 0, 0},
//        {0, 0, 0, 1, 0}
//    };
//    Point start1 = { 0, 0 };
//    Point end1 = { 4, 4 };
//    int pathLength1 = 0;
//
//    Point* path1 = BFS((int*)grid1, start1, end1, &pathLength1);
//
//    assert(path1 != NULL); // Убедитесь, что путь найден
//    assert(pathLength1 > 0); // Длина пути больше нуля
//
//    free(path1);
//
//    // Пример 2: Путь не существует
//    int grid2[5][5] = {
//        {0, 1, 1, 1, 0},
//        {0, 1, 1, 1, 0},
//        {0, 0, 0, 1, 0},
//        {1, 1, 1, 1, 0},
//        {0, 0, 0, 0, 0}
//    };
//    Point start2 = { 0, 0 };
//    Point end2 = { 4, 4 };
//    int pathLength2 = 0;
//
//    Point* path2 = BFS((int*)grid2, start2, end2, &pathLength2);
//
//    assert(path2 == NULL); // Убедитесь, что путь не найден
//
//    // Пример 3: Проверка корректности пути
//    int grid3[5][5] = {
//        {0, 0, 0, 0, 0},
//        {0, 1, 1, 1, 0},
//        {0, 0, 0, 1, 0},
//        {0, 1, 0, 0, 0},
//        {0, 0, 0, 1, 0}
//    };
//    Point start3 = { 0, 0 };
//    Point end3 = { 4, 4 };
//    int pathLength3 = 0;
//
//    Point* path3 = BFS((int*)grid3, start3, end3, &pathLength3);
//
//    assert(path3 != NULL); // Убедитесь что путь найден
//    assert(pathLength3 > 0); // Длина пути больше нуля
//
//    // Проверка правильности конечной точки
//    assert(path3[pathLength3 - 1].x == end3.x && path3[pathLength3 - 1].y == end3.y);
//
//    free(path3);
//}
//
//int main() {
//    test_BFS();
//    printf("true!\n");
//    return 0;
//}