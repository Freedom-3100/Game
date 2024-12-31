#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <png.h>
#include "generation.h"
#include "colision.h"
#define MAX_IMAGES 3

// Определение структуры Player
typedef struct {
    int x;                  // Позиция игрока по оси X
    int y;                  // Позиция игрока по оси Y
    SDL_Texture* texture[MAX_IMAGES];
    int current;
    BSPNode* currentRoom;
} Player;


void init_player(Player* player, int startX, int startY, SDL_Texture* texture[], BSPNode* curr_room);
void render_player(Player* player, SDL_Renderer* renderer, SDL_Rect dstRect);
void move_player(Player* player, const char* direction);
void move_within_room(Player* player, const char* direction);
void handle_input(Player* player);