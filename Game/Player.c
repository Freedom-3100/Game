#include "Player.h"




void init_player(Player* player, int startX, int startY, SDL_Texture* texture[],BSPNode * curr_room)
{
    player->x = startX;
    player->y = startY;        // Начальная защита
    for (int i = 0; i < MAX_IMAGES; i++) {
        player->texture[i] = texture[i]; // Копируем текстуры в массив
    }
    player->currentRoom = curr_room;
    player->current = 0;    // Текстура игрока
}




void render_player(Player* player, SDL_Renderer* renderer, SDL_Rect dstRect)
{
    SDL_RenderCopy(renderer, player->texture[player->current], NULL, &dstRect);
}

void move_player(Player* player, const char* direction) {
    BSPNode* nextRoom = NULL;

    // Проверка направления и наличие двери
    if (strcmp(direction, "up") == 0 && player->currentRoom->door_up) {
        nextRoom = player->currentRoom->up;

    }
    else if (strcmp(direction, "down") == 0 && player->currentRoom->door_down) {
        nextRoom = player->currentRoom->down;
    }
    else if (strcmp(direction, "left") == 0 && player->currentRoom->door_left) {
        nextRoom = player->currentRoom->left;
    }
    else if (strcmp(direction, "right") == 0 && player->currentRoom->door_right) {
        nextRoom = player->currentRoom->right;
    }

    // Если следующая комната найдена, перемещаем игрока
    if (nextRoom != NULL) {
        player->currentRoom = nextRoom; // Переход в новую комнату
        printf("Moved to the room.\n");

        // Обновляем позицию игрока в центре новой комнаты
        player->x = WINDOW_WIDTH / 2; // Предполагаем, что ширина комнаты
        player->y = WINDOW_HIGH / 2; // Предполагаем, что высота комнаты
    }
    else {
        printf("No room to move into.\n");
    }
}

void move_within_room(Player* player, const char* direction) {
    if (strcmp(direction, "up") == 0 && player->y > 0) {
        player->y -= 15; // Перемещение на 5 пикселей вверх
    }
    else if (strcmp(direction, "down") == 0 && player->y < WINDOW_HIGH - 50) {
        player->y += 15; // Перемещение на 5 пикселей вниз
    }
    else if (strcmp(direction, "left") == 0 && player->x > 0) {
        player->x -= 15; // Перемещение на 5 пикселей влево
    }
    else if (strcmp(direction, "right") == 0 && player->x < WINDOW_WIDTH - 50) {
        player->x += 15; // Перемещение на 5 пикселей вправо
    }
}

void handle_input(Player* player) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0); // Выход из программы при закрытии окна
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
                if (player->currentRoom->up && player->currentRoom->door_up) {
                    move_player(player, "up");
                }
                else {
                    move_within_room(player, "up");
                }
                break;
            case SDL_SCANCODE_A:
                if (player->currentRoom->left && player->currentRoom->door_left) {
                    move_player(player, "left");
                }
                else {
                    move_within_room(player, "left");
                }
                break;
            case SDL_SCANCODE_S:
                if (player->currentRoom->down && player->currentRoom->door_down) {
                    move_player(player, "down");
                }
                else {
                    move_within_room(player, "down");
                }
                break;
            case SDL_SCANCODE_D:
                if (player->currentRoom->right && player->currentRoom->door_right) {
                    move_player(player, "right");
                }
                else {
                    move_within_room(player, "right");
                }
                break;
            default:
                break;
            }
        }
    }
}