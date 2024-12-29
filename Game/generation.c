#include "generation.h"


// ������� ��� �������� ����� �������
Room create_room() {
    Room room;
    room.matix_room = malloc(WINDOW_HIGH * WINDOW_WIDTH * sizeof(int));
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HIGH; y++) {
            if (x == 0 || y == WINDOW_HIGH - 1 || y == 0 || x == WINDOW_WIDTH - 1) {
                room.matix_room[y * WINDOW_WIDTH + x] = 1;
            }
            else {
                room.matix_room[y * WINDOW_WIDTH + x] = 0;
            }
        }
    }

    // ������� ��������� ��������� �������� 30 �� 30
    int maxX = WINDOW_WIDTH - TEXTURE_SIZE - 1; // ������������ ������� �� X
    int maxY = WINDOW_HIGH - TEXTURE_SIZE - 1; // ������������ ������� �� Y

     // ������������� ���������� ��������� �����

    for (int i = 0; i < 10; i++) { // ��������, ��������� 10 ���������
        int x = rand() % maxX + 1; // ��������� ������� �� X (�� ������� �������)
        int y = rand() % maxY + 1; // ��������� ������� �� Y (�� ������� �������)

        // ��������� ������� 30x30 ���������
        for (int dx = 0; dx < TEXTURE_SIZE; dx++) {
            for (int dy = 0; dy < TEXTURE_SIZE; dy++) {
                room.matix_room[(y + dy) * WINDOW_WIDTH + (x + dx)] = 1;
            }
        }
    }
    return room;
}


// ������� ��� �������� ������ ���� BSP
BSPNode* create_bsp_node(Room room) {
    BSPNode* node = malloc(sizeof(BSPNode));
    node->room = malloc(sizeof(Room));  // �������� ������ ��� ����� �������.
    *(node->room) = room;               // �������� ������ ������� � ����� ����.
    node->left = NULL;
    node->right = NULL;
    node->up = NULL;
    node->down = NULL;
    node->parent = NULL;
    return node;
}



BSPNode* space_split(int line, int column, BSPNode* parent) {
    // �������� ������ �� �������
    if (line < 0 || column < 0 || line == 6 || column == 6) {
        printf("Stopping recursion at line: %d, column: %d\n", line, column);
        return NULL;
    }

    Room room = create_room(); // ������� ����� �������
    BSPNode* node = create_bsp_node(room);
    node->parent = parent;

    int direction = rand() % 4; // ���������� �����������

    switch (direction) {
    case 0: // ����
        node->door_down = 1;
        node->down = space_split(line + 1, column, node);
        break;
    case 1: // �����
        node->door_left = 1;
        node->left = space_split(line, column - 1, node);
        break;
    case 2: // �����
        node->door_up = 1 ;
        node->up = space_split(line - 1, column, node);
        break;
    case 3: // ������
        node->door_right = 1;
        node->right = space_split(line, column + 1, node);
        break;
    }

    return node; // ���������� ����
}


void free_bsp_node(BSPNode* node) {
    if (node == NULL) {
        return;
    }

    // ������� ���� ��� �������� ����� � ������������ �������� 
    BSPNode** stack = malloc(sizeof(BSPNode*) * 1024); // ������ ������ �����, �������� 1024
    if (!stack) {
        return; // ���������� �������� ������ ��� �����
    }
    int top = -1;

    stack[++top] = node;

    while (top >= 0) {
        BSPNode* curr = stack[top--];

        // ���� ���� �� NULL, ������������ ���
        if (curr != NULL) {
            // ������ �������� ���� � ����
            if (top < 1023) { // ��������� �� ������������ �����
                stack[++top] = curr->left;
                stack[++top] = curr->right;
                stack[++top] = curr->up;
                stack[++top] = curr->down;
            }
            else {
                // � ������ ������������ ����� ����� ���������� ������
                // ��������, �� ������������ ���������� ����
                break;
            }

            // ����������� ������ ��� ������� �������, ���� ��� ���� ��������
            if ((*curr).room->matix_room != NULL) {
                free((*curr).room->matix_room);
            }

            // ����������� ������ ��� ������ ����
            free(curr);
        }
    }

    // ����������� ����
    free(stack);
}


