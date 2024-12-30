#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define TEXTURE_SIZE 30
#define WINDOW_WIDTH 800
#define WINDOW_HIGH 600

typedef struct Room {
    // Позиция комнаты
    int* matix_room;// матрица 800*600 в которой содержится информация об объектах в комнате
} Room;

typedef struct BSPNode {
    Room * room;
    struct BSPNode* left;
    struct BSPNode* right;
    struct BSPNode* up;
    struct BSPNode* down;
    struct BSPNode* parent;
    bool door_up;
    bool door_down;
    bool door_left;
    bool door_right;
} BSPNode;


Room create_room();
BSPNode* create_bsp_node(Room room);
BSPNode* space_split(int line, int column, BSPNode* parent);
void free_bsp_node(BSPNode* node);
BSPNode* generate_doors(BSPNode* node, int doors_high);