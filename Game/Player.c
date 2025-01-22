#include "Player.h"




void init_player(Player* player, int startX, int startY, SDL_Texture* texture[],BSPNode * curr_room)
{
    player->x = startX;
    player->y = startY;        
    for (int i = 0; i < MAX_IMAGES; i++) {
        player->texture[i] = texture[i]; 
    }
    player->currentRoom = curr_room;
    player->health = 3;
    player->current = 0;    
}




void render_player(Player* player, SDL_Renderer* renderer, SDL_Rect dstRect)
{
    SDL_RenderCopy(renderer, player->texture[player->current], NULL, &dstRect);
}

void move_player(Player* player, const char* direction , int * lenght) {
    BSPNode* nextRoom = NULL;


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


    if (nextRoom != NULL) {
        free(player->currentRoom->room->enemy->queue);
        player->currentRoom->room->enemy->queue = NULL;

        player->currentRoom = nextRoom;
        printf("Moved to the room.\n");
        player->x = WINDOW_WIDTH / 2; 
        player->y = WINDOW_HIGH / 2; 
        *lenght = 0;
    }
    else {
        printf("No room to move into.\n");
    }
}



void move_within_room(Player* player, const char* direction) {
    if (strcmp(direction, "up") == 0 && distance(player->x , player->y , player->x, 0) > 0) {
        if (!check_colision(player->x, player->y - 10, player->currentRoom->room->matix_room))
        {
            player->y -= 5;
        }

    }
    else if (strcmp(direction, "down") == 0 && distance(player->x, player->y, player->x, WINDOW_HIGH) > 40) {
        if (!check_colision(player->x, player->y + 10, player->currentRoom->room->matix_room))
        {
            player->y += 5;
        }

    }
    else if (strcmp(direction, "left") == 0 && distance(player->x, player->y, 0, player->y) > 0) {
        if (!check_colision(player->x - 10, player->y, player->currentRoom->room->matix_room))
        {
            player->x -= 5;
        }

    }
    else if (strcmp(direction, "right") == 0 && distance(player->x, player->y, WINDOW_WIDTH, player->y) > 30) {
        if (!check_colision(player->x + 10, player->y, player->currentRoom->room->matix_room))
        {
            player->x += 5;
        }

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
void handle_input(Player* player, int* lenght) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0); 
        }
        check_doors(player);
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
                if (player->currentRoom->door_up) {
                    move_player(player, "up", lenght);
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
                    move_player(player, "left", lenght);
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
                    move_player(player, "down", lenght);
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
                    move_player(player, "right", lenght);
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