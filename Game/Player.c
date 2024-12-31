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

        if (player->currentRoom->up)
        {
            nextRoom = player->currentRoom->up;
        }
        else
        {
            nextRoom = player->currentRoom->parent;
        }
        

    }
    else if (strcmp(direction, "down") == 0 && player->currentRoom->door_down) {
        if (player->currentRoom->down)
        {
            nextRoom = player->currentRoom->down;
        }
        else
        {
            nextRoom = player->currentRoom->parent;
        }
    }
    else if (strcmp(direction, "left") == 0 && player->currentRoom->door_left) {
        if ( player->currentRoom->left)
        {
            nextRoom = player->currentRoom->left;
        }
        else
        {
            nextRoom = player->currentRoom->parent;
        }
    }
    else if (strcmp(direction, "right") == 0 && player->currentRoom->door_right) {
        if (player->currentRoom->right)
        {
            nextRoom = player->currentRoom->right;
        }
        else
        {
            nextRoom = player->currentRoom->parent;
        }
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

float distance(int cord_x_obj_1, int cord_y_obj_1, int cord_x_obj_2, int cord_y_obj_2)
{
    return(sqrt((cord_x_obj_1 - cord_x_obj_2) * (cord_x_obj_1 - cord_x_obj_2) + (cord_y_obj_1 - cord_y_obj_2) * (cord_y_obj_1 - cord_y_obj_2)));
}

void move_within_room(Player* player, const char* direction) {
    if (strcmp(direction, "up") == 0 && distance(player->x , player->y , player->x, 0) > 0) {
        player->y -= 10; // Перемещение на 5 пикселей вверх
    }
    else if (strcmp(direction, "down") == 0 && distance(player->x, player->y, player->x, WINDOW_HIGH) > 40) {
        player->y += 10; // Перемещение на 5 пикселей вниз
    }
    else if (strcmp(direction, "left") == 0 && distance(player->x, player->y, 0, player->y) > 0) {
        player->x -= 10; // Перемещение на 5 пикселей влево
    }
    else if (strcmp(direction, "right") == 0 && distance(player->x, player->y, WINDOW_WIDTH, player->y) > 30) {
        player->x += 10; // Перемещение на 5 пикселей вправо
    }
}

void check_doors(Player* player)
{
        player->currentRoom->door_left = 0;
        player->currentRoom->door_right = 0;
        player->currentRoom->door_up = 0;
        player->currentRoom->door_down = 0;
        if ((player->currentRoom->left  || (player->currentRoom->parent && player->currentRoom == player->currentRoom->parent->right)) && distance(player->x, player->y, 0, WINDOW_HIGH / 2) < 20)
        {
            player->currentRoom->door_left = 1;
        }
        else if ((player->currentRoom->right || (player->currentRoom->parent && player->currentRoom == player->currentRoom->parent->left)) && distance(player->x, player->y, WINDOW_WIDTH, WINDOW_HIGH / 2) < 50)
        {
            player->currentRoom->door_right = 1;
        }
        else if ((player->currentRoom->up || (player->currentRoom->parent && player->currentRoom == player->currentRoom->parent->down)) && distance(player->x, player->y, WINDOW_WIDTH / 2, 0) < 40 )
        {
            player->currentRoom->door_up = 1;
        }
        else if ((player->currentRoom->down || (player->currentRoom->parent && player->currentRoom == player->currentRoom->parent->up)) && distance(player->x, player->y, WINDOW_WIDTH / 2, WINDOW_HIGH) < 60)
        {
            player->currentRoom->door_down = 1;
        }
    
}
void handle_input(Player* player) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0); // Выход из программы при закрытии окна
        }
        check_doors(player);
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
                if (player->currentRoom->door_up) {
                    move_player(player, "up");
                    player->currentRoom->door_up = 0;
                    player->currentRoom->door_left = 0;
                    player->currentRoom->door_down = 0;
                    player->currentRoom->door_right = 0;
                }
                else {
                    move_within_room(player, "up");
                }
                break;
            case SDL_SCANCODE_A:
                if ( player->currentRoom->door_left) {
                    move_player(player, "left");
                    player->currentRoom->door_up = 0;
                    player->currentRoom->door_left = 0;
                    player->currentRoom->door_down = 0;
                    player->currentRoom->door_right = 0;
                }
                else {
                    move_within_room(player, "left");
                }
                break;
            case SDL_SCANCODE_S:
                if ( player->currentRoom->door_down) {
                    move_player(player, "down");
                    player->currentRoom->door_up = 0;
                    player->currentRoom->door_left = 0;
                    player->currentRoom->door_down = 0;
                    player->currentRoom->door_right = 0;
                }
                else {
                    move_within_room(player, "down");

                }
                break;
            case SDL_SCANCODE_D:
                if (player->currentRoom->door_right) {
                    move_player(player, "right");
                    player->currentRoom->door_up = 0;
                    player->currentRoom->door_left = 0;
                    player->currentRoom->door_down = 0;
                    player->currentRoom->door_right = 0;
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