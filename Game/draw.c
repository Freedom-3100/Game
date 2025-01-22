#include "draw.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>


void render_health_bar(SDL_Renderer* renderer, int health, int max_health, Point player_cord) {

    int bar_width = 50;
    int bar_height = 5;
    int x = player_cord.x + 5; 
    int y = player_cord.y - 10; 

    float health_ratio = (float)health / max_health;
    int current_health_width = (int)(bar_width * health_ratio);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect health_bar_bg = { x, y, bar_width, bar_height };
    SDL_RenderFillRect(renderer, &health_bar_bg);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect health_bar = { x, y, current_health_width, bar_height };
    SDL_RenderFillRect(renderer, &health_bar);
}


void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width = surface->w;
    int text_height = surface->h;
    SDL_Rect dstrect = { x, y, text_width, text_height };

    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

static draw_bullet(Enemy * enemy, int* lenght, Point  player_cord, SDL_Renderer* renderer, Damage* damage)
{
    if (enemy->is_life == Life)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect bullet_rect = { enemy->queue[0].cur_cords.x , enemy->queue[0].cur_cords.y  , BULLET_SIZE , BULLET_SIZE };
        SDL_RenderFillRect(renderer, &bullet_rect);
        enemy->queue[0].cur_cords = change_cords_bullet(enemy->queue[0].cur_cords, enemy->queue[0].end_cords, 5);
        enemy = check_collision_bullet(enemy, player_cord, lenght, damage);
    }
    
}


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
        printf("Ошибка инициализации SDL: %s\n", SDL_GetError());
        return;
    }

    SDL_Window* window = SDL_CreateWindow("Roguelike Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HIGH,  
        SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* playerTexture[MAX_IMAGES];

    playerTexture[0] = IMG_LoadTexture(renderer, "images/persona_3.png");
    playerTexture[1] = IMG_LoadTexture(renderer, "images/persona_2.png");
    playerTexture[2] = IMG_LoadTexture(renderer, "images/persona_1.png");
    TTF_Font* font = TTF_OpenFont("C:/Games/sample.ttf", 24);
    BSPNode* root = space_split(2, 2, NULL);
    SDL_Color textColor = { 0, 255, 0, 255 };
    Player player;
    init_player(&player, 300, 400, playerTexture, root); 
    SDL_Rect dstRect = { player.x, player.y, 40, 40 };
    Point player_cord = { player.x, player.y };
    player.currentRoom->room->enemy->distance = distance_to_player(player_cord, player.currentRoom->room->enemy->cur_cords);
    int index_path = 0;
    int stay_time = 0;
    int running = 1;
    int lenght = 0;
    int new_calc = 150;
    Damage damage = No;
    while (running) {
        handle_input(&player, &lenght);
        stay_time++;

        if (stay_time > 10)
        {
            stay_time = 0;
            player.current = 0;
        }
        Point player_cord = { player.x, player.y };
        
        player.currentRoom->room->enemy = state_machine(player.currentRoom->room->enemy, player_cord, player.currentRoom->room->matix_room, &new_calc ,&index_path, &lenght);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        dstRect.x = player.x;
        dstRect.y = player.y;

        draw_rooms(renderer, player.currentRoom);
        draw_enemy(renderer, player.currentRoom->room->enemy->cur_cords);
        draw_bullet(player.currentRoom->room->enemy, &lenght, player_cord, renderer, &damage);
        if (damage == Yes)
        {
            damage = No;
            player.health--;
        }
        render_health_bar(renderer, player.health, 3, player_cord);
        if (player.health <= 0)
        {
            running = 0;
            printf("GAME OVER");
        }
        render_player(&player, renderer, dstRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/30);
    }
    free_bsp_node(root);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}


menu_state showMenu() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    menu_state state = Exit;
    SDL_Window* window = SDL_CreateWindow("Game Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HIGH, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("C:/Games/sample.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = { 255, 255, 255, 255 }; 

    int menu = 1;
    SDL_Event event;

    while (menu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                state = End;
                menu = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    menu = 0;
                    break;
                case SDLK_1:
                    printf("Start Game");

                     state = Game;

                    menu = 0;
                    break;
                case SDLK_2:
                    printf("Options\n");
                    state = Rules;

                    break;
                case SDLK_4:
                    printf("Exit\n");
                    state = Exit;
                    break;
                case SDLK_3:
                    state = End;
                    menu = 0;
                }

            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        if (state == Exit)
        {
            drawText(renderer, font, "Game Menu", 300, 100, textColor);
            drawText(renderer, font, "Press 1. Start Game", 300, 200, textColor);
            drawText(renderer, font, "Press 2. Rules", 300, 300, textColor);
            drawText(renderer, font, "Press 3. End Game", 300, 400, textColor);
            
        }
        else if (state == Rules)
        {
            drawText(renderer, font, "Managment: W,A,S,D", 300, 100, textColor);
            drawText(renderer, font, "White blocks are an obstacle", 300, 200, textColor);
            drawText(renderer, font, "Red blocks are an enemy", 300, 300, textColor);
            drawText(renderer, font, "Yellow blocks are an doors to next room", 300, 400, textColor);
            drawText(renderer, font, "Press 4. Menu", 300, 500, textColor);
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
    return state;
}



