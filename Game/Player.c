#include "Player.h"




void init_player(Player* player, int startX, int startY, SDL_Texture* texture[],BSPNode * curr_room)
{
    player->x = startX;
    player->y = startY;        // ��������� ������
    for (int i = 0; i < MAX_IMAGES; i++) {
        player->texture[i] = texture[i]; // �������� �������� � ������
    }
    player->currentRoom = curr_room;
    player->current = 0;    // �������� ������
}




void render_player(Player* player, SDL_Renderer* renderer, SDL_Rect dstRect)
{
    SDL_RenderCopy(renderer, player->texture[player->current], NULL, &dstRect);
}

void move_player(Player* player, const char* direction) {
    BSPNode* nextRoom = NULL;

    // �������� ����������� � ������� �����
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

    // ���� ��������� ������� �������, ���������� ������
    if (nextRoom != NULL) {
        player->currentRoom = nextRoom; // ������� � ����� �������
        printf("Moved to the room.\n");

        // ��������� ������� ������ � ������ ����� �������
        player->x = WINDOW_WIDTH / 2; // ������������, ��� ������ �������
        player->y = WINDOW_HIGH / 2; // ������������, ��� ������ �������
    }
    else {
        printf("No room to move into.\n");
    }
}

void move_within_room(Player* player, const char* direction) {
    if (strcmp(direction, "up") == 0 && player->y > 0) {
        player->y -= 15; // ����������� �� 5 �������� �����
    }
    else if (strcmp(direction, "down") == 0 && player->y < WINDOW_HIGH - 50) {
        player->y += 15; // ����������� �� 5 �������� ����
    }
    else if (strcmp(direction, "left") == 0 && player->x > 0) {
        player->x -= 15; // ����������� �� 5 �������� �����
    }
    else if (strcmp(direction, "right") == 0 && player->x < WINDOW_WIDTH - 50) {
        player->x += 15; // ����������� �� 5 �������� ������
    }
}

void handle_input(Player* player) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0); // ����� �� ��������� ��� �������� ����
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