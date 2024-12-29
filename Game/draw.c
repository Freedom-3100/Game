#include "draw.h"


static void draw_rooms(SDL_Renderer* renderer, BSPNode* room) {
        //  ��� ��� ���� ������ ������ ��� ���������� �� �������� �������
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // ����� ����
        //SDL_RenderDrawRect(renderer, &rect);
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            for (int y = 0; y < WINDOW_HIGH; y++)
            {
                if (room->room->matix_room[y * WINDOW_WIDTH + x] == 1)
                {
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
    }

static void draw_doors(SDL_Renderer* renderer, BSPNode* room) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // ���� ������ (������)

    int doorWidth = 20; // ������ �����
    int doorHeight = 50; // ������ �����

    // ��������� ����� �����
    if (room->door_up && room->up) {
        SDL_Rect doorUp = { (WINDOW_WIDTH / 2) - (doorWidth / 2), 0, doorWidth, doorHeight };
        SDL_RenderFillRect(renderer, &doorUp);
    }

    // ��������� ����� ����
    if (room->door_down && room->down) {
        SDL_Rect doorDown = { (WINDOW_WIDTH / 2) - (doorWidth / 2), WINDOW_HIGH - doorHeight, doorWidth, doorHeight };
        SDL_RenderFillRect(renderer, &doorDown);
    }

    // ��������� ����� �����
    if (room->door_left && room->left) {
        SDL_Rect doorLeft = { 0, (WINDOW_HIGH / 2) - (doorWidth / 2), doorHeight, doorWidth };
        SDL_RenderFillRect(renderer, &doorLeft);
    }

    // ��������� ����� ������
    if (room->door_right && room->right) {
        SDL_Rect doorRight = { WINDOW_WIDTH - doorHeight, (WINDOW_HIGH / 2) - (doorWidth / 2), doorHeight, doorWidth };
        SDL_RenderFillRect(renderer, &doorRight);
    }
}

void draw()
{
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("������ ������������� SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Roguelike Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HIGH,  
        SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* playerTexture[MAX_IMAGES];

    // �������� ������� ��� �������� (��������������, ��� ����� ����������)
    playerTexture[0] = IMG_LoadTexture(renderer, "C:/Games/persona_3.png");
    playerTexture[1] = IMG_LoadTexture(renderer, "C:/Games/persona_2.png");
    playerTexture[2] = IMG_LoadTexture(renderer, "C:/Games/persona_1.png");
    BSPNode* root = space_split(3, 3, NULL);
    Player player;
    init_player(&player, 300, 400, playerTexture, root); // ������������� ������
    SDL_Rect dstRect = { player.x, player.y, 40, 40 };
    // �������� ������� ����
    int stay_time = 0;
    int running = 1;
    //printf("lox");
    while (running) {
        handle_input(&player);
        stay_time++;

        if (stay_time > 10)
        {
            stay_time = 0;
            player.current = 0;
        }
        // ������� ������
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        dstRect.x = player.x;
        dstRect.y = player.y;
        //root = go_room(root, player);
        draw_rooms(renderer, player.currentRoom);
        draw_doors(renderer, player.currentRoom);
        // ��������� ������
        render_player(&player, renderer, dstRect);
  
        // ���������� ������
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/20);
    }
    // ������������ ��������
    free_bsp_node(root);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}