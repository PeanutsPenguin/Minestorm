#include "game_manager.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MineStorm");

    Player_array*   player = player_array_init(30);
    Game_status*    currentStatus = game_status_init();
    Army*           army = army_init(28);

    Struct_list*    struct_list = struct_list_init(player, currentStatus, army);

    // SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        update_and_draw(struct_list);
    }


    CloseWindow();

    free_game(struct_list);

    return 0;
}