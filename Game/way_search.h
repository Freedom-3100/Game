#pragma once
#include <math.h>
#include <stdbool.h>
#define WINDOW_WIDTH 800
#define WINDOW_HIGH 600
#define ENEMY_SIZE 40
typedef struct {
    int x;
    int y;
} Point;

// Структура для представления узла в очереди
typedef struct {
    Point pt; // Координаты клетки
    int dist; // Расстояние от начальной клетки
    Point prev; // Предшествующая клетка на пути
} Node;

Point* BFS(int* grid, Point start, Point end);