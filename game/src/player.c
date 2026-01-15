#include "game_manager.h"

/*Function to initialize the player Structure*/
Player_array*   player_array_init(unsigned int nbrOfProjectile)
{
    Player_array*   player = (Player_array*)malloc(sizeof(Player_array));                               //Stock the structure in the HEAP memory and get a pointer to it

    player->nbrOfProjectile = nbrOfProjectile;                                                          //Associate the number of projectile in our struct to the number of projectile given
    player->playerOne = player_Init((Vector2){0, 0});                                                   //Initialize the player one in the top left corner
    player->playerTwo = player_Init((Vector2){SCREEN_WIDTH, 0});                                        //Initialize the player two in the top right corner

    GameObject**    array = (GameObject**)malloc(sizeof(GameObject*)*player->nbrOfProjectile);          //Stock the projectile array of the first player in the HEAP memory and get a pointer to it
    GameObject**    arrayTwo = (GameObject**)malloc(sizeof(GameObject*)*player->nbrOfProjectile);       //Stock the projectile array of the second player in the HEAP memory and get a pointer to it

    for (int i = 0; i < player->nbrOfProjectile; i++)                                                   //Create a projectile in every case of both projectile array
    {
        array[i] = projectile_init();
        arrayTwo[i] = projectile_init();
    }

    player->projectileArrayOne = array;                                                                 //Associate the pointer to the first projectile array in our struct with the pointer to the array stock in the HEAP memory
    player->projectileArrayTwo = arrayTwo;                                                              //Associate the pointer to the second projectile array in our struct with the pointer to the array stock in the HEAP memory

    return player;
}

/*Function to initialize a player*/
GameObject*     player_Init(Vector2 position)
{
    GameObject* player = (GameObject*)malloc(sizeof(GameObject));                                       //Stock the structure in the Heap memory and get a pointer to it
    if (player == NULL)                                                                                 //If the pointer is NULL, we gob back and return the NULL value
        return NULL;

    player->active = true;                                                                              //Set our player to active 
    player->numberOfSides = PLAYER_NUMBER_SIDES;                                                        //Associate the number of sides of the player to the number of side defined
    player->center = position;                                                                          //Set the player position to the given position
    player->rotation = 0;                                                                               //Initialize the rotation to 0
    player->life = PLAYER_LIFE;                                                                         //Associate the number of life given to the number of life defined
    player->speed = 0;                                                                                  //Set the speed to 0 so the player don't move forward at the beginning 
    player->coolDown = 0;                                                                               //Set the player coolDown to 0
    player->tag = E_PLAYER;                                                                             //Add the right tag

    Vector2*  matrix = (Vector2*)malloc(sizeof(Vector2)*player->numberOfSides);                         //Stock the array of all the vertex of our form in the Heap memory and get a pointer to it
    Vector2*  normalVec = (Vector2*)malloc(sizeof(Vector2)*player->numberOfSides);                      //Same with the array of normal vector
    float* angleArray = (float*)malloc(sizeof(float)*player->numberOfSides);                            //Same with the angle array
    unsigned int* radiusArray = (unsigned int*)malloc(sizeof(unsigned int)*player->numberOfSides);      //Same with the radius array

    for (int i = 0; i < player->numberOfSides; i++)                                                     
    {                                             
        radiusArray[i] = PLAYER_RADIUS;                                                                 //Initialize the radius array                              
    }

    angleArray[0] = 0;                                                                                  //Initialize the angle array
    angleArray[1] = 120;
    angleArray[2] = 180;
    angleArray[3] = 240;


    player->radiusArray = radiusArray;                                                                  //Associate the pointer to the radius array with the pointer to the array we stocked in the HEAP memory
    player->angleArray = angleArray;                                                                    //Same but with angle array
    player->vertexMatrix = matrix;                                                                      //Same but with vertex array
    player->normalVecArray = normalVec;                                                                 //Same but with the array of normal Vector

    return player;
}

/*Function to initialize a projectile*/
GameObject*     projectile_init()
{
    GameObject* player_projectile = (GameObject*)malloc(sizeof(GameObject));                                        //Stock the structure un the heap memory and get a pointer to it
    if (player_projectile == NULL)                                                                                  //If the pointer is NULL return NULL
        return NULL;

    player_projectile->active = false;                                                                              //Initialize the projectile to false    
    player_projectile->center = (Vector2){-100, -100};                                                              //Set the position of the projectile out of the screen
    player_projectile->coolDown = 0;                                                                                //Set the coolDown to 0
    player_projectile->life = PROJECTILE_LIFE;                                                                      //Associate the projectile life in our struct to the number of life defined
    player_projectile->numberOfSides = PROJECTILE_NUMBER_SIDES;                                                     //Associate the projectile number of side in our struct to the number of side defined
    player_projectile->rotation = 0;                                                                                //Set the rotation to 0
    player_projectile->speed = PROJECTILE_SPEED;                                                                    //Associate the projectile speed to the speed defined
    player_projectile->tag = E_PLAYER_PROJECTILE;
    
    Vector2*  matrix = (Vector2*)malloc(sizeof(Vector2)*player_projectile->numberOfSides);                          //Stock the array of all the vertex of our form in the Heap memory and get a pointer to it
    Vector2*  normalVec = (Vector2*)malloc(sizeof(Vector2)*player_projectile->numberOfSides);                       //Same with the array of normal vector
    float* angleArray = (float*)malloc(sizeof(float)*player_projectile->numberOfSides);                             //Same with the angle array
    unsigned int* radiusArray = (unsigned int*)malloc(sizeof(unsigned int)*player_projectile->numberOfSides);       //Same with the radius array

    for (int i = 0; i < player_projectile->numberOfSides; i++)                                              
    {
        angleArray[i] = (360/player_projectile->numberOfSides) * i;                                                 //Initialize the angle array                        
        radiusArray[i] = PROJECTILE_RADIUS;                                                                         //Initialize the radius array to the radius defined
        matrix[i] = (Vector2){0, 0};                                                                                //Initialize the vertex array with nul values
    }

    player_projectile->radiusArray = radiusArray;                                                                   //Associate the pointer to the radius array with the pointer to the array we stocked in the HEAP memory
    player_projectile->angleArray = angleArray;                                                                     //Same but with angle array
    player_projectile->vertexMatrix = matrix;                                                                       //Same but with vertex array
    player_projectile->normalVecArray = normalVec;                                                                  //Same but with normal vector array

    return player_projectile;                                                                                       //Return the pointer to our GameObject
}

/*Function to update the player*/
void            update_playerOne(Struct_list* list)
{
    list->player->playerOne->coolDown += list->currentStatus->deltaTime;                                                            //Increase the player coolDown every frame

    update_vertex_array(list->player->playerOne);                                                                                   //Update the vertex array of the player every frame
    update_vector_normal(list->player->playerOne);                                                                                  //Update the normal vector array of the player every frame
    border_collision(list->player->playerOne);                                                                                      //Prevent the player to get out of the screen
    stable_rotation(list->player->playerOne);                                                                                       //Keep a value between -360 and 360 for the player rotation

    if (list->currentStatus->isScreenSwitching && list->player->playerOne->life > 0 && list->army->enemy_left > 0)              //If we have to switch screen and the player is tuch by an enemi we restart the current level
    {
        reset_enemies(list);
        list->player->playerOne->active = true;
        switch_level(list, list->currentStatus->currentScreen);
    }

    else if(list->currentStatus->isScreenSwitching && list->player->playerOne->life == 0 && list->player->playerTwo->life == 0)     //If we have to switch screen and both player are dead, switch to Game over screen
        switch_level(list, E_GAMEOVER);
    else
    {
        if (!list->player->playerOne->active)                                                                                       //If the player is inactive, get out of the function
            return;

        control_player_one(list);                                                                                                   //Function to 'use' the player with the right keyBindings
        check_collision_enemis_player(list, list->player->playerOne);                                                               //Check the collisions between the enemies and the player
        list->player->playerOne->center = move_to_vector(list->player->playerOne->center,                                           //Move the player forward
                                                        list->player->playerOne->vertexMatrix[0], 
                                                        list->player->playerOne->speed*list->currentStatus->deltaTime);

        for (unsigned int i = 0; i < list->player->nbrOfProjectile; i++)                                                            //Check all the projectile        
        {
            if (!list->player->projectileArrayOne[i]->active)                                                                       //If the projectile is inactive, skip to the next one
                continue;

            projectile_update(list->player->projectileArrayOne[i], list->currentStatus);                                            //Update the projectile
            check_collision_enemis_projectile(list->player->projectileArrayOne[i], list->army, list->player->playerOne,list);            //Check the collision between the projectile and the enemies 
        }
    }
}

/*Function to control the player*/
void            control_player_one(Struct_list* list)
{
    if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_R))                                                              //If we press D and we're not going forward we turning to the left
    {
        list->player->playerOne->rotation -= PLAYER_ROTATION_SPEED * list->currentStatus->deltaTime;
    }

    if (IsKeyDown(KEY_G) && !IsKeyDown(KEY_R))                                                              //If we press G and we're not going forward we turning to the right
    {
        list->player->playerOne->rotation += PLAYER_ROTATION_SPEED * list->currentStatus->deltaTime;
    }

    if (IsKeyDown(KEY_R) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_G))                                         //If we press R and we're not turning, we're going forward
    {
        list->player->playerOne->speed = PLAYER_SPEED;
    }
    else
        list->player->playerOne->speed *= 0.999f;                                                           //Reduce the player speed everytime so it creates a friction

    if ((IsKeyPressed(KEY_E) || IsKeyPressed(KEY_T)) && list->player->playerOne->coolDown > 5)              //If we press E or T and the cool down is superior to 5, we teleports ourselve to a random position
    {
        list->player->playerOne->center = player_teleportation(list->player->playerOne);
        list->player->playerOne->coolDown = 0;
    }

    if (IsKeyPressed(KEY_F))                                                                                //If we press F we creating a projectile
        projectile_create(list->player->projectileArrayOne, 
                            list->player->nbrOfProjectile, 
                            list->player->playerOne);
}

/*Function to check the collision between a projectile and the enemies*/
void            check_collision_enemis_projectile(GameObject* projectile, Army* army, GameObject* player, Struct_list* list)
{
    for (unsigned int j = 0; j < army->nb_ennemies; j++)                                                                                                        //Checking for all enemies 
    {
        if(!army->ennemies[j]->active)                                                                                             //If the enemy is inactive skip to the next one
            continue;
        
        if (army->ennemies[j]->tag != E_SPAWN)                                                                                                                  //If the enemy ism't a spawn point
        {
            if(SAT_between_convexe_and_concave(projectile, army->ennemies[j]))                                                                                  //If there is a collision between the projectile and the enemy
            {
                update_score(list,army->ennemies[j]);
                projectile->active = false;                                                                                                                     //Destroy the projectile
                if ((army->ennemies[j]->life == 1 || army->ennemies[j]->life == 2)                                                                              //If the enemy is a Big or medium and he is a FloatingMine or a MagneticMine
                    && (army->ennemies[j]->tag == E_FLOATING || army->ennemies[j]->tag == E_MAGNETIC))
                {   
                    army->ennemies[j + j % 7+ 1] = gameobject_reset(army->ennemies[j]->tag, army->ennemies[j]->life + 1, army->ennemies[j + j % 7+ 1]);         //Create two enemies with a smaller size
                    army->ennemies[j + j % 7 + 2] = gameobject_reset(army->ennemies[j]->tag, army->ennemies[j]->life + 1, army->ennemies[j + j % 7 + 2]);
                    army->ennemies[j]->active = false;                                                                                                          //Destroy the enemy
                    army->enemy_left -= 1;                                                                                                                  //-1 on the enemy left counter
                }
                else if ((army->ennemies[j]->life == 1 || army->ennemies[j]->life == 2)                                                                         //If the enemy is a big one or a medium one and he is contains a fireball
                        && (army->ennemies[j]->tag == E_FIREBALL_MINE || army->ennemies[j]->tag == E_MAGNETIC_FIREBALL))
                {   
                    army->ennemies[j + j % 7+ 1] = gameobject_reset(army->ennemies[j]->tag, army->ennemies[j]->life + 1, army->ennemies[j + j % 7+ 1]);         //Create two enemies with a smaller size
                    army->ennemies[j + j % 7 + 2] = gameobject_reset(army->ennemies[j]->tag, army->ennemies[j]->life + 1, army->ennemies[j + j % 7 + 2]);
                    army->enemy_left -= 1;                                                                                                                  //-1 on the enemy left counter
                    army->ennemies[j] = reset_to_fireball(army->ennemies[j]->life, army->ennemies[j], player);                                                  //Reset the original enemy into a fireball
                }
                else if ((army->ennemies[j]->life == 3) && (army->ennemies[j]->tag == E_FIREBALL_MINE || army->ennemies[j]->tag == E_MAGNETIC_FIREBALL))       //If the enemy is a small one and he contains a fireBall
                {
                    army->ennemies[j] = reset_to_fireball(army->ennemies[j]->life, army->ennemies[j], player);                                                  //Reset the enemy into a fireball
                    army->enemy_left -= 1;                                                                                                                  //-1 on the enemy left counter
                }
                else if ((army->ennemies[j]->life == 3) && (army->ennemies[j]->tag == E_FLOATING || army->ennemies[j]->tag == E_MAGNETIC))                      //If the enemy is a small one and he is a FLoating Mine or a MagneticMine
                {   
                    army->ennemies[j]->active = false;                                                                                                          //Destroy the enemy
                    army->enemy_left -= 1;                                                                                                                  //-1 on the enemy left counter 
                }   
                else                                                                                                                                            
                    army->ennemies[j]->active = false;                                                                                                          //Destroy the enemy
            }
        }
    }
}

/*Function to check the collision between enemies and the player One*/
void            check_collision_enemis_player(Struct_list* list, GameObject* player)
{
    for (unsigned int j = 0; j < list->army->nb_ennemies; j++)                                          //Doing this for every enemies
    {
        if(!list->army->ennemies[j]->active)                                                            //If the enemy is inactive, skip to the next one
            continue;

        if (list->army->ennemies[j]->tag != E_SPAWN)                                                    //If the enemy is not a spawn point
        {
            if(SAT_between_convexe_and_concave(player, list->army->ennemies[j]))                        //If there is a collision between the given player and the enemy
            {
                player->life -= 1;                                                                      //Player life -1
                player->speed = 0;                                                                      //Set his speed to 0
                player->active = false;                                                                 //Set the player insactive
                if (list->currentStatus->bothPlayer)                                                    //If there is two player in the game
                {   
                    if (!list->player->playerOne->active && !list->player->playerTwo->active)           //Check if both are dead
                        list->currentStatus->isScreenSwitching = true;                                  //If yes switch screen
                }
                else 
                {
                    list->currentStatus->isScreenSwitching = true;                                      //If there is only one player, switch screen
                }
                    
            }
        }
    }
}

/*Function to create a projectile for player one*/
void            projectile_create(GameObject** projectile_array, unsigned int nbrOfProjectile, GameObject* Player)
{
    for (int i = 0; i < nbrOfProjectile; i++)                           //Doing this for every projectile
    {
        if (!projectile_array[i]->active)                               //If the projectile is inactive
        {
            projectile_array[i]->active = true;                         //Set it to active
            projectile_array[i]->rotation = Player->rotation;           //Set his rotation to the rotation of the player that shotted him
            projectile_array[i]->center = Player->vertexMatrix[0];      //Set his position to the first vertex of the player that shooted him
            return;
        }
    }
}

/*Function to update the player Two*/
void            update_playerTwo(Struct_list* list) 
{
    list->player->playerTwo->coolDown += list->currentStatus->deltaTime;                                                                //Update the cooldown of the player two in seconds

    update_vertex_array(list->player->playerTwo);                                                                                       //Update the vertex array of the player two
    update_vector_normal(list->player->playerTwo);                                                                                      //Update the normal vector array of the player two
    border_collision(list->player->playerTwo);                                                                                          //Prevent the player two to get out of the screen
    stable_rotation(list->player->playerTwo);                                                                                           //Prevent the player roation to stay between -360 adn 360

    if (list->currentStatus->isScreenSwitching && list->player->playerTwo->life > 0 && list->army->enemy_left > 0)                                                    //If we have to switch screen and the player has nore than one life
    {
        reset_enemies(list);
        list->player->playerTwo->active = true;
        switch_level(list, E_LVLONE);
    }
    else if(list->currentStatus->isScreenSwitching && list->player->playerOne->life == 0 && list->player->playerTwo->life == 0)         //If we have to switch screen and the player has no nore life we switch to game over screen
        switch_level(list, E_GAMEOVER);
    else
    {
        if (!list->player->playerTwo->active)                                                                                           //If the player is inactive, get out of the function
            return;

        control_player_two(list);                                                                                                       //Function to 'use' the player with the right keyBindings
        check_collision_enemis_player(list, list->player->playerTwo);                                                                   //Check the collisions between the enemies and the player
        list->player->playerTwo->center = move_to_vector(list->player->playerTwo->center, 
                                                        list->player->playerTwo->vertexMatrix[0], 
                                                        list->player->playerTwo->speed*list->currentStatus->deltaTime);

        for (unsigned int i = 0; i < list->player->nbrOfProjectile; i++)                                                                //Check all the projectile     
        {
            if (!list->player->projectileArrayTwo[i]->active)                                                                           //If the projectile is inactive skip to the mext one
                continue;   

            projectile_update(list->player->projectileArrayTwo[i], list->currentStatus);                                                //Update the projectile 
            check_collision_enemis_projectile(list->player->projectileArrayTwo[i], list->army, list->player->playerTwo,list);                //Check if there is a collision between the projectile and an enemi
        }
    }
}

/*Function to control the Player Two*/
void            control_player_two(Struct_list* list)
{
    if (IsKeyDown(KEY_J) && !IsKeyDown(KEY_I))                                                              //If we press J and we're not going forward we turning to the left
        list->player->playerTwo->rotation -= PLAYER_ROTATION_SPEED * list->currentStatus->deltaTime;

    if (IsKeyDown(KEY_L) && !IsKeyDown(KEY_I))                                                              //If we press L and we're not going forward we turning to the left
        list->player->playerTwo->rotation += PLAYER_ROTATION_SPEED * list->currentStatus->deltaTime;

    if (IsKeyDown(KEY_I) && !IsKeyDown(KEY_J) && !IsKeyDown(KEY_L))                                         //If we press I and we're not turning, we're going forward
        list->player->playerTwo->speed = PLAYER_SPEED;
    else
        list->player->playerTwo->speed *= 0.999f;                                                           //Reduce the player speed everytime so it creates a friction

    if ((IsKeyPressed(KEY_U) || IsKeyPressed(KEY_O)) && list->player->playerTwo->coolDown > 5)              //If we press U or O and the cool down is superior to 5, we teleports ourselve to a random position
    {
        list->player->playerTwo->center = player_teleportation(list->player->playerTwo);
        list->player->playerTwo->coolDown = 0;
    }

    if (IsKeyPressed(KEY_K))                                                                                //If we press K we creating a projectile
        projectile_create(list->player->projectileArrayTwo, 
                            list->player->nbrOfProjectile, 
                            list->player->playerTwo);
}

/*Function to update a Projectile*/
void            projectile_update(GameObject* projectile, Game_status* currentStatus)
{
    if (!projectile->active)                                                                //If the projectile is inactive, get out of the function
        return;

    border_collision(projectile);                                                           //Prevent the projectile to get out of the screen
    update_vertex_array(projectile);                                                        //Update the vertex array of the projectile
    update_vector_normal(projectile);                                                       //Update the normal vector array of the projectile 
    
    projectile->center = move_to_vector(projectile->center,                                 //Move the projectile forward
                                        projectile->vertexMatrix[0], 
                                        projectile->speed*currentStatus->deltaTime);

    if(projectile->coolDown <= 0.75)                                                        //Update the projectile cooldown in seconds
        projectile->coolDown += currentStatus->deltaTime;

    if (projectile->coolDown > 0.75)                                                        //If the projectile cooldown is superior to 0.75, destroy the projectile
    {   
        projectile->active = false;
        projectile->coolDown = 0;
    }
}

/*Function to create a random position*/
Vector2         player_teleportation(GameObject* player)
{
    float newx = GetRandomValue(PLAYER_RADIUS, SCREEN_WIDTH - PLAYER_RADIUS);           //Create a random .x
    float newy = GetRandomValue(PLAYER_RADIUS, SCREEN_HEIGHT - PLAYER_RADIUS);          //Create a random .y

    return (Vector2){newx, newy};                                                       //Return a vector with the .x and the .y
}

void            free_playerOne(Struct_list* list)
{    
    for (unsigned int i = 0; i < list->player->nbrOfProjectile; i++)
    {
        free(list->player->projectileArrayOne[i]->angleArray);
        free(list->player->projectileArrayOne[i]->normalVecArray);
        free(list->player->projectileArrayOne[i]->radiusArray);
        free(list->player->projectileArrayOne[i]->vertexMatrix);
        free(list->player->projectileArrayOne[i]);
    }

    free(list->player->projectileArrayOne);
    free(list->player->playerOne->vertexMatrix);
    free(list->player->playerOne->angleArray);
    free(list->player->playerOne->normalVecArray);
    free(list->player->playerOne->radiusArray);
    free(list->player->playerOne);
}

void            free_playerTwo(Struct_list* list)
{    
    for (unsigned int i = 0; i < list->player->nbrOfProjectile; i++)
    {
        free(list->player->projectileArrayTwo[i]->angleArray);
        free(list->player->projectileArrayTwo[i]->normalVecArray);
        free(list->player->projectileArrayTwo[i]->radiusArray);
        free(list->player->projectileArrayTwo[i]->vertexMatrix);
        free(list->player->projectileArrayTwo[i]);
    }
    free(list->player->projectileArrayTwo);
    free(list->player->playerTwo->vertexMatrix);
    free(list->player->playerTwo->angleArray);
    free(list->player->playerTwo->normalVecArray);
    free(list->player->playerTwo->radiusArray);
    free(list->player->playerTwo);
}