#pragma once
#include "Player.h"
#include "generation.h"
#include "enemy.h"
typedef enum{Game, Rules, Exit, End}menu_state;


menu_state showMenu();
void draw();