#include "draw.h"


static void draw_enemy(SDL_Renderer* renderer, Point enemy_cord)

{
    SDL_SetRenderDrawColor(renderer, 255, 0,0, 255);
    SDL_Rect enemy_rect = { enemy_cord.x , enemy_cord.y  , ENEMY_SIZE , ENEMY_SIZE };
    SDL_RenderFillRect(renderer, &enemy_rect);
}


static void draw_rooms(SDL_Renderer* renderer, BSPNode* room) {
        room = generate_doors(room, 50);
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            for (int y = 0; y < WINDOW_HIGH; y++)
            {
                if (room->room->matix_room[y * WINDOW_WIDTH + x] == 1)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
                else if (room->room->matix_room[y * WINDOW_WIDTH + x] == 2)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
    }

void draw()
{
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("������ ������������� SDL: %s\n", SDL_GetError());
        return;
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
    BSPNode* root = space_split(2, 2, NULL);
    Player player;
    init_player(&player, 300, 400, playerTexture, root); // ������������� ������
    SDL_Rect dstRect = { player.x, player.y, 40, 40 };
    Point player_cord = { player.x, player.y };
    player.currentRoom->room->enemy->distance = distance_to_player(player_cord, player.currentRoom->room->enemy->cur_cords);
    // �������� ������� ����
    int index_path = 0;
    int stay_time = 0;
    int running = 1;
    int new_calc = 150;
    //printf("lox");
    while (running) {
        handle_input(&player);
        stay_time++;

        if (stay_time > 10)
        {
            stay_time = 0;
            player.current = 0;
        }
        Point player_cord = { player.x, player.y };
        player.currentRoom->room->enemy = state_machine(player.currentRoom->room->enemy, player_cord, player.currentRoom->room->matix_room, &new_calc ,&index_path);
        // ������� ������
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        dstRect.x = player.x;
        dstRect.y = player.y;
        //root = go_room(root, player);

        draw_rooms(renderer, player.currentRoom);
        draw_enemy(renderer, player.currentRoom->room->enemy->cur_cords);
        //draw_doors(renderer, player.currentRoom);
        // ��������� ������
        render_player(&player, renderer, dstRect);
  
        // ���������� ������
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/30);
    }
    // ������������ ��������
    free_bsp_node(root);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}