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

// ��������� ��� ������������� ���� � �������
typedef struct {
    Point pt; // ���������� ������
    int dist; // ���������� �� ��������� ������
    Point prev; // �������������� ������ �� ����
} Node;

Point* BFS(int* grid, Point start, Point end);