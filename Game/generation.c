#include "generation.h"


// Функция для создания новой комнаты
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

    // Вставка случайных квадратов размером 30 на 30
    int maxX = WINDOW_WIDTH - TEXTURE_SIZE - 1; // Максимальная позиция по X
    int maxY = WINDOW_HIGH - TEXTURE_SIZE - 1; // Максимальная позиция по Y

     // Инициализация генератора случайных чисел

    for (int i = 0; i < 10; i++) { // Например, вставляем 10 квадратов
        int x = rand() % maxX + 1; // Случайная позиция по X (не включая границы)
        int y = rand() % maxY + 1; // Случайная позиция по Y (не включая границы)

        // Заполняем квадрат 30x30 единицами
        for (int dx = 0; dx < TEXTURE_SIZE; dx++) {
            for (int dy = 0; dy < TEXTURE_SIZE; dy++) {
                room.matix_room[(y + dy) * WINDOW_WIDTH + (x + dx)] = 1;
            }
        }
    }
    return room;
}


// Функция для создания нового узла BSP
BSPNode* create_bsp_node(Room room) {
    BSPNode* node = malloc(sizeof(BSPNode));
    node->room = malloc(sizeof(Room));  // Выделяем память для новой комнаты.
    *(node->room) = room;               // Копируем данные комнаты в новый узел.
    node->left = NULL;
    node->right = NULL;
    node->up = NULL;
    node->down = NULL;
    node->parent = NULL;
    node->door_up = 0;
    node->door_right = 0;
    node->door_left = 0;
    node->door_down = 0;
    return node;
}



BSPNode* space_split(int line, int column, BSPNode* parent) {
    // Проверка выхода за границы
    if (line < 0 || column < 0 || line == 5 || column == 5) {
        printf("Stopping recursion at line: %d, column: %d\n", line, column);
        return NULL;
    }

    Room room = create_room(); // Создаем новую комнату
    BSPNode* node = create_bsp_node(room);
    node->parent = parent;

    int direction = rand() % 4; // Генерируем направление

    switch (direction) {
    case 0: // вниз
        if (node->parent && node == node->parent->up)
        {
            node->left = space_split(line, column - 1, node);
        }
        else
        {
            node->down = space_split(line + 1, column, node);
        }
        
        break;
    case 1: // влево
        if (node->parent && node == node->parent->right)
        {
            node->down = space_split(line + 1, column, node);
        }
        else
        {
            node->left = space_split(line, column - 1, node);
        }
        break;
    case 2: // вверх
        if (node->parent && node == node->parent->down)
        {
            node->right = space_split(line, column + 1, node);
        }
        else
        {
            node->up = space_split(line - 1, column, node);
        }
        break;
    case 3: // вправо
        if (node->parent && node == node->parent->left)
        {
            node->up = space_split(line - 1, column, node);
        }
        else
        {
            node->right = space_split(line, column + 1, node);
        }
        break;
    }

    return node; // Возвращаем узел
}


BSPNode* generate_doors(BSPNode* node, int doors_high)
{
    if (node->right || (node->parent && node == node->parent->left))
    {
        for (int x = 1; x < 11; x++)
        {
            for (int y = WINDOW_HIGH / 2 - doors_high / 2; y < WINDOW_HIGH / 2 + doors_high / 2; y++)
            {
                node->room->matix_room[y * WINDOW_WIDTH + (WINDOW_WIDTH - x)] = 2;
            }
        }
        
    }
    if (node->left || (node->parent && node == node->parent->right))
    {
        for (int x = 1; x < 11; x++)
        {
            for (int y = WINDOW_HIGH / 2 - doors_high / 2; y < WINDOW_HIGH / 2 + doors_high / 2; y++)
            {
                node->room->matix_room[y * WINDOW_WIDTH +  x] = 2;
            }
        }
    }
    if (node->up || (node->parent && node == node->parent->down))
    {
        for (int y = 1; y < 11; y++)
        {
            for (int x = WINDOW_WIDTH / 2 - doors_high / 2; x < WINDOW_WIDTH / 2 + doors_high / 2; x++)
            {
                node->room->matix_room[y * WINDOW_WIDTH + x] = 2;
            }
        }

    }

    if (node->down || (node->parent && node == node->parent->up))
    {
        for (int y = 1; y < 11; y++)
        {
            for (int x = WINDOW_WIDTH / 2 - doors_high / 2; x < WINDOW_WIDTH / 2 + doors_high / 2; x++)
            {
                node->room->matix_room[(WINDOW_HIGH - y ) * WINDOW_WIDTH + x] = 2;
            }
        }
    }
    return node;
}

void free_bsp_node(BSPNode* node) {
    if (node == NULL) {
        return;
    }

    // Создаем стек для хранения узлов с динамическим размером 
    BSPNode** stack = malloc(sizeof(BSPNode*) * 1024); // Задать размер стека, например 1024
    if (!stack) {
        return; // Невозможно выделить память для стека
    }
    int top = -1;

    stack[++top] = node;

    while (top >= 0) {
        BSPNode* curr = stack[top--];

        // Если узел не NULL, обрабатываем его
        if (curr != NULL) {
            // Ставим дочерние узлы в стек
            if (top < 1023) { // Проверяем на переполнение стека
                stack[++top] = curr->left;
                stack[++top] = curr->right;
                stack[++top] = curr->up;
                stack[++top] = curr->down;
            }
            else {
                // В случае переполнения стека можно обработать ошибку
                // Например, не обрабатывать оставшиеся узлы
                break;
            }

            // Освобождаем память для матрицы комнаты, если она была выделена
            if ((*curr).room->matix_room != NULL) {
                free((*curr).room->matix_room);
            }

            // Освобождаем память для самого узла
            free(curr);
        }
    }

    // Освобождаем стек
    free(stack);
}


