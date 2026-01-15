#include "game_manager.h"

void        draw_grid(void)
{
    DrawRectangleRounded((Rectangle){ BACKGROUND_POS, BACKGROUND_POS, RECT_WIDTH, RECT_HEIGHT }, CORNER_RADIUS, 0, MIDNIGHT_BLUE);

    for (int posX = BACKGROUND_POS; posX < BACKGROUND_POS + RECT_WIDTH; posX += GRID_SIZE)
    {
        DrawLine(posX, BACKGROUND_POS, posX, BACKGROUND_POS + RECT_HEIGHT, LIGHT_BLUE);
    }
    for (int posY = BACKGROUND_POS; posY < BACKGROUND_POS + RECT_HEIGHT; posY += GRID_SIZE)
    {
        DrawLine(BACKGROUND_POS, posY, BACKGROUND_POS + RECT_WIDTH, posY, LIGHT_BLUE);
    }
}

void        draw_border(void)
{
    DrawRectangleRoundedLines((Rectangle){ BACKGROUND_POS, BACKGROUND_POS, RECT_WIDTH, RECT_HEIGHT }, CORNER_RADIUS, 0, 50, YELLOW);
    DrawRectangleRoundedLines((Rectangle){ BACKGROUND_POS, BACKGROUND_POS, RECT_WIDTH, RECT_HEIGHT }, CORNER_RADIUS, 0, 10, WHITE);

    DrawRectangleRounded((Rectangle){ 15, 1115, 770, 70 }, 3.0f, 0, GREEN_BLUE);
    DrawRectangle(15, 1115, 770, 40, GREEN_BLUE);
    DrawRectangle(15, 1115, 770, 5, RED);

    DrawRectangleRounded((Rectangle){LIFE_RECT_X, LIFE_RECT_Y, LIFE_RECT_WIDTH, LIFE_RECT_HEIGHT}, CORNER_RADIUS, 0, TURQUOISE);
    DrawText("PLAYER ONE", 105, 80, 20, BLACK);

    DrawRectangleRounded((Rectangle){LIFE_RECT2_X, LIFE_RECT_Y, LIFE_RECT_WIDTH, LIFE_RECT_HEIGHT}, CORNER_RADIUS, 0, GREEN);
    DrawText("PLAYER TWO", 550, 80, 20, BLACK);
}

void        player_life(Struct_list* list)
{
    switch (list->player->playerOne->life)
    {
        case 3:
        DrawPoly((Vector2){220,130},3,20,180,BLACK);
        case 2:
        DrawPoly((Vector2){170,130},3,20,180,BLACK);
        case 1:
        DrawPoly((Vector2){120,130},3,20,180,BLACK);
        break;
    }

    switch (list->player->playerTwo->life)
    {
        case 3:
        DrawPoly((Vector2){670,130},3,20,180,BLACK);
        case 2:
        DrawPoly((Vector2){620,130},3,20,180,BLACK);
        case 1:
        DrawPoly((Vector2){570,130},3,20,180,BLACK);
        break;
    }
}

void        info_player(void)
{
    DrawText("Rotate: 'D' and 'G'", 80, 180, 20, WHITE);
    DrawText("Move: 'R'", 80, 200, 20, WHITE);
    DrawText("Shoot:'F'", 80, 220, 20, WHITE);
    DrawText("Teleport: 'E' or 'T'", 80, 240, 20, WHITE);

    DrawText("Rotate: 'J' and 'L'", 525, 180, 20, WHITE);
    DrawText("Move: 'I'", 525, 200, 20, WHITE);
    DrawText("Shoot:'K'", 525, 220, 20, WHITE);
    DrawText("Teleport: 'U' or 'O'", 525, 240, 20, WHITE);
}

void        main_menu(void)
{
    draw_grid();

    DrawText("MineStorm", 95, 450, 120, WHITE);

    DrawText("Press 'F':        Solo mode", 150, 650, 30, BLUE);
    DrawText("Press 'K':        Twoplayers mode", 150, 700, 30, GREEN);
    DrawText("Press 'Esc':      Exit", 150, 750, 30, RED);
}

void        in_game(void)
{
    draw_grid();
}

void        pause_menu(void)
{
    draw_grid();

    DrawText("PAUSE", 200, 450, 120, WHITE);

    DrawText("Press 'SPACE':        Resume", 150, 610, 30, BLUE);
    DrawText("Press 'Q':      Back to main menu", 150, 660, 30, RED);
}

void        gameover_menu(Struct_list* list)
{

    int     score = 0;
    
    draw_grid();

    DrawText("GAME OVER", 95, 450, 100, WHITE);

    DrawText("SCORE : ", 255, 550, 40, GREEN);
    DrawText(TextFormat("%04i" , list->currentStatus->score), 425, 550, 40, WHITE);
    DrawText("Press 'Q':      Back to main menu", 150, 700, 30, BLUE);
}

void        draw_score(Struct_list* list)
{
    DrawText("SCORE :", 50, 1135, 40, WHITE);
    DrawText(TextFormat("%04i" ,list->currentStatus->score), 220, 1135, 40, WHITE);
}

void        update_score(Struct_list* list, GameObject* enemi)
{   
    switch (enemi->tag)
    {
        case E_FLOATING:
            if (enemi->life == 1)
            {
                list->currentStatus->score += 100;
                break;
            } else if (enemi->life == 2)
            {
                list->currentStatus->score += 135;
                break;
            } else if (enemi->life == 3)
            {
                list->currentStatus->score += 200;
                break;
            }
            break;
        case E_FIREBALL_MINE:
            if (enemi->life == 1)
            {
                list->currentStatus->score += 325;
            } else if (enemi->life == 2)
            {
                list->currentStatus->score += 360;
            } else if (enemi->life == 3)
            {
                list->currentStatus->score += 425;
            }
            break;
        case E_MAGNETIC:
            if (enemi->life == 3)
            {
                list->currentStatus->score += 500;
            } else if (enemi->life == 2)
            {
                list->currentStatus->score += 535;
            } else if (enemi->life == 1)
            {
                list->currentStatus->score += 600;
            }
            break;
        case E_MAGNETIC_FIREBALL:
            if (enemi->life == 3)
            {
                list->currentStatus->score += 750;
            } else if (enemi->life == 2)
            {
                list->currentStatus->score += 585;
            } else if (enemi->life == 1)
            {
                list->currentStatus->score += 850;
            }
            break;
        case E_FIREBALL:
            if (enemi->life == 1)
            {
                list->currentStatus->score += 110;
            }
            break;
        default:
            break;
    }
}

void        draw_level(Struct_list* list)
{
    switch (list->currentStatus->currentScreen)
    {
        case(E_LVLONE):
        {
            DrawText("Level One", 300, 80, 30, WHITE);
        }break;

        case(E_LVLTWO):
        {
            DrawText("Level Two", 300, 80, 30, WHITE);
        }break;

        case(E_LVLTHREE):
        {
            DrawText("Level Three", 300, 80, 30, WHITE);
        }break;

        case(E_LVLFOUR):
        {
            DrawText("Level Three", 300, 80, 30, WHITE);
        }break;
    default:
        break;
    }
}