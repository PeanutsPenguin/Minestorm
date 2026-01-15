#include "game_manager.h"

/*Function to initialize the structure GameStatus*/
Game_status*    game_status_init(void)
{
    Game_status*    currentStatus = (Game_status*)malloc(sizeof(Game_status));          //Stock the structure in the HEAP memory and get a pointer to it

    currentStatus->deltaTime = GetFrameTime();                                          //Get the time between wo frames
    currentStatus->currentScreen = E_TITLE;                                             //Starting on the title screen
    currentStatus->isGamePaused = false;                                                //The game is not paused by default
    currentStatus->isScreenSwitching = false;                                           //The game is not switching screen by default
    currentStatus->bothPlayer = false;                                                //There is no player at the beginning so it's false by default
    currentStatus->score = 0;

    return currentStatus;                                                               //Return the pointer to our struct
}

/*Function to initialize the structure Army*/
Army*           army_init(int nb_ennemies)
{
    Army* army = (Army*)malloc(sizeof(Army));                                                       //Stock the structure in the HEAP memor and get a pointer to it 

    if (army == NULL)                                                                               //If the pointer is NULL, return NULL
        return NULL;

    army->nb_ennemies = nb_ennemies;                                                                //Associate the number of enemies in our struct to the number of enemies given 
    army->enemy_left = nb_ennemies;                                                                 //The number of enemy left is the same number because no enemy died yet

    GameObject** ennemies = (GameObject**)malloc(sizeof(GameObject*) * army->nb_ennemies);          //Stock the array of enemies in the HEAP memory and get a pointer to it

    for (int i = 0; i < army->nb_ennemies; i++)                                                     //Create a spawn point for every enemies
    {
        ennemies[i] = spawn_points_init((Vector2){GetRandomValue(BACKGROUND_POS, RECT_WIDTH - 35), 
                                                  GetRandomValue(200, RECT_HEIGHT - 50)});
    }
    
    army->ennemies = ennemies;                                                                      //Associate the pointer to the enemy array in our struct to the pointer we just get

    return army;                                                                                    //Return the pointer to our struct
}   

/*Function to initilize the structure StructList*/
Struct_list*    struct_list_init(Player_array* player, Game_status* currentStatus, Army* army)
{
    Struct_list* struct_list = (Struct_list*)malloc(sizeof(Struct_list));       //Stock the struct in the HEAP memory and get a pointer to it

    //Associate the pointers in our structure to all the pointers to the structure we just created
    struct_list->player = player;
    struct_list->currentStatus = currentStatus;
    struct_list->army = army;

    return struct_list;                                                         //Return the pointer to our struct
}

/*Functoin to update and draw the game, depends on wich screen is active*/
void            update_and_draw(Struct_list* list)
{
    switch (list->currentStatus->currentScreen)
    {
        case(E_TITLE):
        {
            update_title_screen(list);
            draw_title_screen(list);
        }break;

        case(E_LVLONE):
        {
            update_game(list);
            draw_game(list);
        }break;

        case(E_LVLTWO):
        {
            update_game(list);
            draw_game(list);
        }break;

        case(E_LVLTHREE):
        {
            update_game(list);
            draw_game(list);
        }break;

        case(E_LVLFOUR):
        {
            update_game(list);
            draw_game(list);
        }break;

        case(E_GAMEOVER):
        {
            update_game_over_screen(list);
            draw_game_over_screen(list);
        }
        default:
            break;
    }
}

/*Function to draw the Game when we are in a level*/
void            draw_game(Struct_list* list)
{
    BeginDrawing();

    if (list->currentStatus->isScreenSwitching)                                     //If we're switching screens, Draw only the player
        draw_switch_screen(list);
    else 
    {
        ClearBackground(BLACK);

        in_game();                                                                //Draw the Background
        player_life(list);
        in_game();
        draw_level(list);
        draw_GameObject(list->player->playerOne, TURQUOISE);                        //Draw the player in TURQUOISE

        for (int i = 0; i < list->army->nb_ennemies; i++)                           //Draw all the enemies
        {   
            if (list->army->ennemies[i]->tag == E_SPAWN)                            //If the enemy is a spawn point, only draw a circle
            {
                DrawCircle(list->army->ennemies[i]->center.x, 
                           list->army->ennemies[i]->center.y, 
                           POINT_RADIUS, RED);
                continue;
            }

            draw_GameObject(list->army->ennemies[i], RED);
        }
        
        if (list->player->playerTwo->active)                                        //If the player two is alive, Draw the player two in GREEN
            draw_GameObject(list->player->playerTwo, GREEN);

        for (int i = 0; i < list->player->nbrOfProjectile; i++)                     //Draw all the projectiles that the players shoot
        {
            draw_GameObject(list->player->projectileArrayOne[i], BLUE);

            if (list->player->playerTwo->active)
                draw_GameObject(list->player->projectileArrayTwo[i], GREEN);
        }
    }

        draw_border();
        player_life(list);
        DrawText("| | Press 'SPACE'", 600, 1140, 20, YELLOW);
        draw_score(list);

    if (list->currentStatus->isGamePaused)                                          //If the game is paused, draw the pause menu
    {
        pause_menu();
        draw_border();
        info_player();
        player_life(list);
        draw_score(list);
    }   
    EndDrawing();
}

/*Function to draw a GameObject*/
void            draw_GameObject(GameObject* object, Color color)
{
    if (!object->active)                                    
        return;
        
    for (unsigned int i = 0; i < object->numberOfSides - 1; i++)                                                            //Doing this as many times as there is sides in our form but -1 
    {                                                                                                                       //because we want to draw the last one with the first one

        DrawLine(object->vertexMatrix[i].x, object->vertexMatrix[i].y,                                                      //Draw a line starting from a vertex 
        object->vertexMatrix[i+1].x, object->vertexMatrix[i+1].y,                                                           //to the next vertex
        color);                                                                                                             //Draw this line in the wanted color
    }

    DrawLine(object->vertexMatrix[object->numberOfSides -1].x,                                                              //Draw a line starting from the last vertex
             object->vertexMatrix[object->numberOfSides -1].y,                                                      
    object->vertexMatrix[0].x, object->vertexMatrix[0].y,                                                                   //to the first vertex
    color);                                                                                                                 //Draw this line in the wanted color
    
    if (object->tag == E_FIREBALL_MINE || object->tag == E_MAGNETIC_FIREBALL)                                                                                     //If the object is a Fireball mine or a magnetic fireabll, draw a circle in it
    {
        DrawCircleLines(object->center.x, object->center.y, 
                       (FIREBALL_MINE_RADIUS / object->life)/8, RED);
    }                                                       

    if (IsKeyDown(KEY_C))
    {
        DrawLine(object->center.x, object->center.y,                                                                         //Draw a line starting from the center of the form 
        object->vertexMatrix[0].x, object->vertexMatrix[0].y,                                                                //to the first vertex
        RED);                                                                                                                //Draw this line in RED
    }                                                               
}

/*Function to draw the title screen*/
void            draw_title_screen(Struct_list* list)
{
    BeginDrawing();

    if (list->currentStatus->isScreenSwitching)             //If we're switching screen, draw only the player
        draw_switch_screen(list);
    else
    {
        ClearBackground(BLACK);
        main_menu();
        draw_border();                                       //Draw the main menu
        info_player();                                      //Draw the player info
        player_life(list);                                  //Draw the player life
    }

    EndDrawing();
}

/*Function to draw the Game Over screen*/
void            draw_game_over_screen(Struct_list* list)
{
    BeginDrawing();

    if(list->currentStatus->isScreenSwitching)          //If we're switching screen, draw only the player
        draw_switch_screen(list);
    else 
    {
        gameover_menu(list);
        draw_border();                                //Draw the game over menu
        info_player();                                  //Draw the player info
    }


    EndDrawing();
}

/*Function to update the game if we are in a level*/
void            update_game(Struct_list* list)
{
    list->currentStatus->deltaTime = GetFrameTime();                                //Get the time between two frames

    if (IsKeyPressed(KEY_SPACE))                                                    //If Space is pressed, pause the game
        list->currentStatus->isGamePaused = !list->currentStatus->isGamePaused;

    if (!list->currentStatus->isGamePaused)                                         //If the game is not paused
    {
        update_playerOne(list);                                                     //Update the player one
        update_ennemies(list);                                                      //Update the enemies

        if (list->currentStatus->bothPlayer)                                        //If there is two player in the game, update the second player
            update_playerTwo(list);

        if (list->army->enemy_left <= 0)                                            //If all enemies are dead, switch screen
            list->currentStatus->isScreenSwitching = true;

        if (list->currentStatus->currentScreen == E_LVLONE                          //If all enemies are dead and we're on lvl one, got to lvl two
            && list->currentStatus->isScreenSwitching 
            && list->army->enemy_left == 0)
        { 
            switch_level(list,E_LVLTWO);  
        }

        if (list->currentStatus->currentScreen == E_LVLTWO                          //If all enemies are dead and we're on lvl two, got to lvl three
            && list->currentStatus->isScreenSwitching 
            && list->army->enemy_left == 0)
        {
            switch_level(list,E_LVLTHREE);   
        }

        if (list->currentStatus->currentScreen == E_LVLTHREE                        //If all enemies are dead and we're on lvl three, got to lvl four
            && list->currentStatus->isScreenSwitching 
            && list->army->enemy_left == 0)
        { 
            switch_level(list,E_LVLFOUR);        
        }

        if (list->currentStatus->currentScreen == E_LVLFOUR                         //If all enemies are dead and we're on lvl four, restart the level
            && list->currentStatus->isScreenSwitching 
            && list->army->enemy_left == 0)
        {
            switch_level(list,E_LVLFOUR);          
        }
    }
    else 
    {
        if(IsKeyPressed(KEY_Q))
        {
            list->currentStatus->currentScreen = E_TITLE;
;        }
    }
}

/*Function to update the title screen*/
void            update_title_screen(Struct_list* list)
{
    list->currentStatus->isGamePaused = false;
    list->currentStatus->deltaTime = GetFrameTime();                                                //Get the time Beween two frames
    list->player->playerOne->rotation += PLAYER_ROTATION_SPEED*list->currentStatus->deltaTime;      //Make the player rotate constantly to get an animtion
    list->player->playerTwo->rotation += PLAYER_ROTATION_SPEED*list->currentStatus->deltaTime;      //Make the second player rotate constantly to get an animtion
    update_vertex_array(list->player->playerOne);                                                   //Update the player one vertexes
    update_vertex_array(list->player->playerTwo);                                                   //Update the player two vertexes

    if(IsKeyPressed(KEY_F))                                                                         //If F is pressed, only the player one will be playable
    {
        list->player->playerTwo->active = false;
        list->player->playerTwo->life = 0;
        list->currentStatus->bothPlayer = false;
        list->player->playerOne->active = true;
        list->currentStatus->isScreenSwitching = true;
    }

    if (IsKeyPressed(KEY_K))                                                                        //If we press K, both player are playable
    {
        list->currentStatus->bothPlayer = true;
        list->player->playerOne->active = true;
        list->player->playerTwo->active = true;
        list->currentStatus->isScreenSwitching = true;
    }

    if (list->currentStatus->isScreenSwitching)                                                     //Switch to the level one once a key is pressed 
    {
        switch_level(list, E_LVLONE);
    }
}

/*Function to update the Game over screen*/
void            update_game_over_screen(Struct_list* list)  
{
    if(IsKeyPressed(KEY_Q))                                                     //If we press Q, it goes on the title screen
    {
        list->player->playerOne->life = PLAYER_LIFE;
        list->player->playerTwo->life = PLAYER_LIFE;
        reset_enemies(list);
        list->currentStatus->currentScreen = E_TITLE;
    }

    if (IsKeyPressed(KEY_A))                                                    //If we press A, it restart to the level one
    {
        list->player->playerOne->life = PLAYER_LIFE;
        list->player->playerOne->active = true;
        list->player->playerOne->center = (Vector2){0, 0};
        if (list->currentStatus->bothPlayer)
        {
            list->player->playerTwo->active = true;
            list->player->playerTwo->life = PLAYER_LIFE;
            list->player->playerTwo->center = (Vector2){SCREEN_WIDTH, 0};
        }
        reset_enemies(list);
        list->currentStatus->isScreenSwitching = true;
    }

    if(list->currentStatus->isScreenSwitching)
    {
        switch_level(list, E_LVLONE);
    }
        
}

/*Function to have an animation between screens*/
void            switch_level(Struct_list* list, Screen nextLevel)
{
    if (!list->currentStatus->bothPlayer)                                                               //If the player two is inactive, move only the player one to the center
    {
        list->player->playerOne->center = move_to_vector(list->player->playerOne->center, 
                                                    (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT/2}, 
                                                    PLAYER_SPEED*list->currentStatus->deltaTime);

        if ((list->player->playerOne->center.x == SCREEN_WIDTH/2)                                       //When the player reaches the center, switch to the given screen
            && (list->player->playerOne->center.y == SCREEN_HEIGHT/2))
        {
            list->currentStatus->isScreenSwitching = false;
            list->currentStatus->currentScreen = nextLevel;
            reset_enemies(list); 
        }
    }
    else                                                                                                //If there is two player, move both to the center
    {
        list->player->playerTwo->active = true;
        list->player->playerOne->active = true;
        list->player->playerOne->center = move_to_vector(list->player->playerOne->center, 
                                                    (Vector2){SCREEN_WIDTH/2 - PLAYER_RADIUS*2, SCREEN_HEIGHT/2}, 
                                                    PLAYER_SPEED*list->currentStatus->deltaTime);
        
        list->player->playerTwo->center = move_to_vector(list->player->playerTwo->center, 
                                                        (Vector2){SCREEN_WIDTH/2 + PLAYER_RADIUS*2, SCREEN_HEIGHT/2}, 
                                                        PLAYER_SPEED*list->currentStatus->deltaTime);

        if ((list->player->playerOne->center.x == SCREEN_WIDTH/2 - PLAYER_RADIUS*2                      //When both reach the center, switch to the given screen
            && list->player->playerOne->center.y == SCREEN_HEIGHT/2)
            && (list->player->playerTwo->center.x == SCREEN_WIDTH/2 + PLAYER_RADIUS*2
            && list->player->playerTwo->center.y == SCREEN_HEIGHT/2))
        {
            list->currentStatus->isScreenSwitching = false;
            list->currentStatus->currentScreen = nextLevel;
            reset_enemies(list); 
        }
    }
}

/*Function to draw the animation*/
void            draw_switch_screen(Struct_list* list)
{
    ClearBackground(BLACK);
    in_game();
    draw_border();
    draw_GameObject(list->player->playerOne, TURQUOISE);        //Draw the player one in TURQUOISE
    if(list->player->playerTwo->active)                         //Draw tbe player two in GREEN if he's alive
        draw_GameObject(list->player->playerTwo, GREEN);
}

/*Function to reset a given GameObject into another*/
GameObject*     gameobject_reset(Type tag, unsigned int life, GameObject* toReset)
{
    switch(tag)                                             //Reset the gameObject into the GameObject associated with the given tag
    {
        case(E_FIREBALL_MINE):
        {
            reset_to_fireballMine(tag, life, toReset);
            return toReset;
        }break;
        
        case(E_FLOATING):
        {
            reset_to_floatingMine(tag, life, toReset);
            return toReset;
        }break;

        case(E_MAGNETIC):
        {
            reset_to_magneticMine(tag, life, toReset);
            return toReset;
        }break;
    
        case(E_MAGNETIC_FIREBALL):
        {
            reset_to_magneticFireballMine(tag, life, toReset);
            return toReset;
        }break;

        case(E_SPAWN):
        {
            reset_to_spawnPoints(tag, toReset);
            return toReset;
        }break;
    }
}

/*Function to free all the malloc*/
void            free_game(Struct_list* list)
{
    free_playerOne(list);
    free_playerTwo(list);
    free_ennemies(list);
    free(list->army);
    free(list->currentStatus);
    free(list->player);
    free(list);
}