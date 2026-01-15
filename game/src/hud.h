#pragma once 
#include "game_manager.h"

#define LIGHT_BLUE (Color){18, 165, 248, 50}
#define MIDNIGHT_BLUE (Color){0, 0, 51, 255}
#define GREEN_BLUE (Color){0, 40, 144, 255}
#define TURQUOISE (Color){0, 255, 249, 255}

#define CORNER_RADIUS 0.1f
#define GRID_SIZE 75
#define BACKGROUND_POS 25
#define RECT_WIDTH 750
#define RECT_HEIGHT 1150

#define LIFE_RECT_X 70
#define LIFE_RECT2_X 515
#define LIFE_RECT_Y 70
#define LIFE_RECT_WIDTH 210
#define LIFE_RECT_HEIGHT 90

typedef struct struct_list_s Struct_list;

void        draw_grid(void);
void        draw_border(void);
void        player_life(Struct_list* list);
void        info_player(void);
void        main_menu(void);
void        in_game(void);
void        pause_menu(void);
void        gameover_menu(Struct_list* list);
void        draw_score(Struct_list* list);
void        update_score(Struct_list* list, GameObject* enemi);
void        draw_level(Struct_list* list);