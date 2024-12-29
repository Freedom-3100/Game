#include "draw.h"


static void draw_rooms(SDL_Renderer* renderer, BSPNode* room) {
        //  вот тут надо короче думать как нарисовать по заданной матрице
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Белый цвет
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Цвет дверей (желтый)

    int doorWidth = 20; // Ширина двери
    int doorHeight = 50; // Высота двери

    // Отрисовка двери вверх
    if (room->door_up && room->up) {
        SDL_Rect doorUp = { (WINDOW_WIDTH / 2) - (doorWidth / 2), 0, doorWidth, doorHeight };
        SDL_RenderFillRect(renderer, &doorUp);
    }

    // Отрисовка двери вниз
    if (room->door_down && room->down) {
        SDL_Rect doorDown = { (WINDOW_WIDTH / 2) - (doorWidth / 2), WINDOW_HIGH - doorHeight, doorWidth, doorHeight };
        SDL_RenderFillRect(renderer, &doorDown);
    }

    // Отрисовка двери влево
    if (room->door_left && room->left) {
        SDL_Rect doorLeft = { 0, (WINDOW_HIGH / 2) - (doorWidth / 2), doorHeight, doorWidth };
        SDL_RenderFillRect(renderer, &doorLeft);
    }

    // Отрисовка двери вправо
    if (room->door_right && room->right) {
        SDL_Rect doorRight = { WINDOW_WIDTH - doorHeight, (WINDOW_HIGH / 2) - (doorWidth / 2), doorHeight, doorWidth };
        SDL_RenderFillRect(renderer, &doorRight);
    }
}

void draw()
{
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Ошибка инициализации SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Roguelike Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HIGH,  
        SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* playerTexture[MAX_IMAGES];

    // Загрузка текстур для анимации (предполагается, что файлы существуют)
    playerTexture[0] = IMG_LoadTexture(renderer, "C:/Games/persona_3.png");
    playerTexture[1] = IMG_LoadTexture(renderer, "C:/Games/persona_2.png");
    playerTexture[2] = IMG_LoadTexture(renderer, "C:/Games/persona_1.png");
    BSPNode* root = space_split(3, 3, NULL);
    Player player;
    init_player(&player, 300, 400, playerTexture, root); // Инициализация игрока
    SDL_Rect dstRect = { player.x, player.y, 40, 40 };
    // Основной игровой цикл
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
        // Очистка экрана
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        dstRect.x = player.x;
        dstRect.y = player.y;
        //root = go_room(root, player);
        draw_rooms(renderer, player.currentRoom);
        draw_doors(renderer, player.currentRoom);
        // Отрисовка игрока
        render_player(&player, renderer, dstRect);
  
        // Обновление экрана
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/20);
    }
    // Освобождение ресурсов
    free_bsp_node(root);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}