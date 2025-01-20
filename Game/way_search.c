#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "way_search.h"


static int isSafe(int row, int col, int* grid, int** visited) {
    return (row >= 0 && row < WINDOW_HIGH && col >= 0 && col < WINDOW_WIDTH && grid[row * WINDOW_WIDTH + col] == 0 && !visited[row][col]);
}


// Функция для восстановления пути
static Point* printPath(Point** prev, Point end) {
    Point* path = (Point*)malloc(WINDOW_HIGH * WINDOW_WIDTH * sizeof(Point));
    int pathLength = 0;

    // Восстанавливаем путь, начиная с конечной точки
    for (Point at = end; at.x != -1 && at.y != -1; at = prev[at.x][at.y]) {
        path[pathLength++] = at;
    }

    // Уменьшаем размер пути до фактической длины
    Point* result = (Point*)malloc(pathLength * sizeof(Point));
    for (int i = 0; i < pathLength; i++) {
        result[i] = path[i];
    }
    free(path); // Освобождаем память временного массива
    return result;
}


Point* BFS(int* grid, Point start, Point end) {
    // Выделяем память для матрицы visited
    int** visited = (int**)malloc(WINDOW_HIGH * sizeof(int*));
    for (int i = 0; i < WINDOW_HIGH; i++) {
        visited[i] = (int*)malloc(WINDOW_WIDTH * sizeof(int));
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            visited[i][j] = 0; // Инициализация всех значений нулями
        }
    }

    // Выделяем память для матрицы prev
    Point** prev = (Point**)malloc(WINDOW_HIGH * sizeof(Point*));
    for (int i = 0; i < WINDOW_HIGH; i++) {
        prev[i] = (Point*)malloc(WINDOW_WIDTH * sizeof(Point));
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            prev[i][j] = (Point){ -1, -1 }; // Инициализация предшественников
        }
    }

    // Направления движения (вверх, вниз, влево, вправо)
    int rowDir[] = { -1, 1, 0, 0 };
    int colDir[] = { 0, 0, -1, 1 };

    // Создаем очередь для BFS
    Node* queue = (Node*)malloc(WINDOW_HIGH * WINDOW_WIDTH * sizeof(Node));
    int front = 0, rear = 0;

    // Добавляем начальную точку в очередь и отмечаем её как посещенную
    queue[rear++] = (Node){ start, 0, {-1, -1} };
    visited[start.x][start.y] = 1;

    while (front < rear) {
        Node current = queue[front++];

        // Если достигли конечной точки, восстанавливаем путь
        if (current.pt.x == end.x && current.pt.y == end.y) {
            Point* path = printPath(prev, end);
            free(queue); // Освобождаем память очереди

            // Освобождаем память для visited и prev
            for (int i = 0; i <  WINDOW_HIGH; i++) {
                free(visited[i]);
                free(prev[i]);
            }
            free(visited);
            free(prev);

            return path;
        }

        // Проверяем все четыре направления
        for (int i = 0; i < 4; i++) {
            int newRow = current.pt.x + rowDir[i];
            int newCol = current.pt.y + colDir[i];

            if (isSafe(newRow, newCol, grid, visited)) {
                visited[newRow][newCol] = 1; // Отмечаем как посещенную
                queue[rear++] = (Node){ {newRow, newCol}, current.dist + 1, current.pt }; // Добавляем в очередь
                prev[newRow][newCol] = current.pt; // Сохраняем предшественника
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
