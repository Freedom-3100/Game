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

static int isSafe(int col, int row, int* grid, int** visited) {
    // ���������, ��������� �� ������� ������ � �������� ������ � �������� �� ���
    return (col >= 0 && col < WINDOW_WIDTH && row >= 0 && row < WINDOW_HIGH && grid[row * WINDOW_WIDTH + col] == 0 && !check_colision_enemy(col,row,grid) && !visited[row][col]);
}

Point* printPath(Point** prev, Point end, int* pathLength) {
    // ��������������� ���� �� �������� �����
    Point* path = (Point*)malloc(WINDOW_HIGH * WINDOW_WIDTH * sizeof(Point));
    *pathLength = 0;

    for (Point at = end; at.x != -1 && at.y != -1; at = prev[at.y][at.x]) {
        path[(*pathLength)++] = at;
    }

    // �������������� ���� ��� ����������� �������
    Point* result = (Point*)malloc(*pathLength * sizeof(Point));
    for (int i = 0; i < *pathLength; i++) {
        result[i] = path[*pathLength - 1 - i];
    }
    free(path); // ����������� ������ ���������� �������
    return result;
}

Point* BFS(int* grid, Point start, Point end, int* pathLength) {
    // �������� ������ ��� ������� visited
    int** visited = (int**)malloc(WINDOW_HIGH * sizeof(int*));
    for (int i = 0; i < WINDOW_HIGH; i++) {
        visited[i] = (int*)malloc(WINDOW_WIDTH * sizeof(int));
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            visited[i][j] = 0; // �������������� ��� ������ ��� ������������
        }
    }

    // �������� ������ ��� ������� prev
    Point** prev = (Point**)malloc(WINDOW_HIGH * sizeof(Point*));
    for (int i = 0; i < WINDOW_HIGH; i++) {
        prev[i] = (Point*)malloc(WINDOW_WIDTH * sizeof(Point));
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            prev[i][j] = (Point){ -1, -1 }; // �������������� ���������������� ��� �����������
        }
    }

    // ����������� �������� (�����, ����, �����, ������)
    int rowDir[] = { -1, 1, 0, 0 }; // ��������� �� y
    int colDir[] = { 0, 0, -1, 1 }; // ��������� �� x

    // ������� ������� ��� BFS
    Node* queue = (Node*)malloc(WINDOW_HIGH * WINDOW_WIDTH * sizeof(Node));
    int front = 0, rear = 0;

    // ��������� ��������� ����� � �������
    queue[rear++] = (Node){ start, 0, {-1, -1} };
    visited[start.y][start.x] = 1;

    while (front < rear) {
        Node current = queue[front++];

        // ���� �������� �������� �����, ��������������� ����
        if (current.pt.x == end.x && current.pt.y == end.y) {
            Point* path = printPath(prev, end, pathLength);
            free(queue); // ����������� ������ �������

            // ����������� ������ ��� visited � prev
            for (int i = 0; i < WINDOW_HIGH; i++) {
                free(visited[i]);
                free(prev[i]);
            }
            free(visited);
            free(prev);

            return path;
        }

        // ��������� ��� ��������� ����������� ��������
        for (int i = 0; i < 4; i++) {
            int newRow = current.pt.y + rowDir[i];
            int newCol = current.pt.x + colDir[i];

            if (isSafe(newCol, newRow, grid, visited)) {
                visited[newRow][newCol] = 1; // �������� ��� ����������
                queue[rear++] = (Node){ {newCol, newRow}, current.dist + 1, current.pt }; // ��������� � �������
                prev[newRow][newCol] = current.pt; // ���������� ���������������
            }
        }
    }

    // ���� ���� �� ������
    free(queue); // ����������� ������ �������


    // ����������� ������ ��� visited � prev
    for (int i = 0; i < WINDOW_HIGH; i++) {
        free(visited[i]);
        free(prev[i]);
    }
    free(visited);
    free(prev);

    return NULL;
}

//// �������� �������
//void test_BFS() {
//    // ������ 1: ���� ����������
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
//    assert(path1 != NULL); // ���������, ��� ���� ������
//    assert(pathLength1 > 0); // ����� ���� ������ ����
//
//    free(path1);
//
//    // ������ 2: ���� �� ����������
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
//    assert(path2 == NULL); // ���������, ��� ���� �� ������
//
//    // ������ 3: �������� ������������ ����
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
//    assert(path3 != NULL); // ��������� ��� ���� ������
//    assert(pathLength3 > 0); // ����� ���� ������ ����
//
//    // �������� ������������ �������� �����
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