#include "game_manager.h"

/*Update the array of enemies*/
void            update_ennemies(Struct_list* list)
{
    for (int i = 0; i < list -> army -> nb_ennemies; i++)                                                               //Doing this for all enemies
    {
        list -> army->ennemies[i] -> coolDown += list -> currentStatus -> deltaTime;                                    //Get the time in seconds

        switch (list -> currentStatus -> currentScreen)                                                                     //Check the current level
        {
            case (E_LVLONE):
            {
                spawn_lvl_one(i, list -> army);                                                                           //Spawn the right enemies for the first level
            }break;
            
            case (E_LVLTWO):
            {
                spawn_lvl_two(i, list -> army);                                                                           //Spawn the right enemies for the second level
            }break;

            case (E_LVLTHREE):
            {
                spawn_lvl_three(i, list -> army);                                                                         //Spawn the right enemies for the third level
            }break;

            case (E_LVLFOUR):
            {
                spawn_lvl_four(i, list -> army);                                                                          //Spawn the right enemies for the fourth level
            }break;

            default:
                break;
        }

        switch (list -> army -> ennemies[i] -> tag)                                                                           //Check the tag of the enemy
        {
            case (E_FLOATING):
            {
                update_basic_mine(list->army->ennemies[i], list -> currentStatus);                                      //If it's a floating mine, use the basic mine update
            }break;

            case (E_FIREBALL_MINE):
            {
                update_basic_mine(list->army->ennemies[i], list -> currentStatus);                                     //If it's a fireball mine, use the basic mine update
            }break;

            case (E_MAGNETIC):
            {
                update_magnetic_mine(list -> army -> ennemies[i], list);            //If it's a magnetic mine, use the magnetic mine update
            }break;

            case (E_MAGNETIC_FIREBALL):
            {
                update_magnetic_mine(list -> army -> ennemies[i], list);   //If it's a Magnetic fireball mine, use the Magnetic mine update
            }break;
            
            case(E_FIREBALL):
            {
                update_fireBall(list, list -> army -> ennemies[i]);                                                         //If it's a Fireball, use the Fireball update
            }
            default:
                break;
        }
    }
}

/*Update function for the floating and fireball mines*/
void            update_basic_mine(GameObject* mine, Game_status* status)
{
    if (!mine -> active)                                                          //If the mine is not active, get out of the function
        return;

    border_collision(mine);                                                     //Prevent the mine Object to get out of the screen
    update_vertex_array(mine);                                                  //Update the values of the vertex array of the mine object
    update_vector_normal(mine);                                                 //Update the values of the vector normal array of the mine object


    mine->center = move_to_vector(mine -> center,                                 //Move forward frame
                                        mine -> vertexMatrix[0], 
                                        mine -> speed * status -> deltaTime);
}

/*Update function for the magnetic mines*/
void            update_magnetic_mine(GameObject* mine, Struct_list* list)
{
    if (!mine -> active)                                                                                  //If the mine is inactive, get ouf of the function
        return;

    border_collision(mine);                                                                             //Prevent the mine to get out of the screen
    update_vertex_array(mine);                                                                          //Update the vertex array of the mine
    update_vector_normal(mine);                                                                         //Update the normal vector array for the mine

    if (list->currentStatus->bothPlayer)                                                                //If we there is two player
    {
        float distanceOne =  sqrt(powf(list -> player -> playerOne -> center.x - mine -> center.x, 2)           //Calculate the distance between the mine and the player one
                                + powf(list -> player -> playerOne -> center.y - mine -> center.y, 2));

        float distanceTwo =  sqrt(powf(list -> player -> playerTwo -> center.x - mine -> center.x, 2)           //Calculate the distance between the mine and the player two
                                + powf(list -> player -> playerTwo -> center.y - mine -> center.y, 2));
        
        if (distanceOne < distanceTwo && list -> player -> playerOne -> active)                               //If the player one is alive and he is closer than the player two, the the mine goes on player one
        {
             mine -> center =  move_to_vector(mine -> center,                                    
                                  list -> player -> playerOne -> center, 
                                  mine -> speed * list -> currentStatus -> deltaTime);   
        }
        else if (distanceTwo < distanceOne && list -> player -> playerTwo -> active)                          //If the player two is alive and he is closer than the player two, the the mine goes on player two
        {
               mine -> center =  move_to_vector(mine -> center,                                    
                                  list -> player -> playerTwo -> center, 
                                  mine -> speed * list -> currentStatus -> deltaTime); 
        }
        else if (!list -> player -> playerOne -> active)                                                      //If the player one is dead, the mine goes on the player two
        {
            mine -> center =  move_to_vector(mine -> center,                                    
                                list -> player -> playerTwo -> center, 
                                mine -> speed * list -> currentStatus -> deltaTime);
        }
        else if (!list -> player -> playerTwo -> active)                                                      //If the player two is dead, the mine goes on the player one
        {
            mine -> center =  move_to_vector(mine -> center,                                    
                                list -> player -> playerOne -> center, 
                                mine -> speed * list -> currentStatus -> deltaTime);
        }

    }
    else                                                                                                //If there is only one player, the mine goes on player one
    {
        mine -> center =  move_to_vector(mine -> center,                                    
                                    list -> player -> playerOne -> center, 
                                    mine -> speed * list -> currentStatus -> deltaTime);
    }
}

/*Reset all enemies to spawn Points*/
void            reset_enemies(Struct_list* list)
{
    list -> army -> enemy_left = list -> army -> nb_ennemies;
    for (unsigned int i = 0; i < list -> army -> nb_ennemies; i++)
    {
        if (list -> army -> ennemies[i] -> tag != E_SPAWN)                                                        //If the enemy is not already a Spawn Point
        {
            reset_to_spawnPoints(E_SPAWN, list -> army -> ennemies[i]);                                         //Call the function to reset the enmy into a SpawnPoint
        }

        list -> army -> ennemies[i] -> center = (Vector2){GetRandomValue(BACKGROUND_POS, RECT_WIDTH - 35),        //Create a new center for the pawn points
                                                    GetRandomValue(200, RECT_HEIGHT - 50)};
        list -> army->ennemies[i] -> coolDown = 0;                                                              //Reset the enemy cool down
        list -> army -> ennemies[i] -> active = true;                                                             //Set the enemy back to true
    }
}

/*Create the enemies for the first level*/
void            spawn_lvl_one(int index, Army* army)
{              
    if (index % 7 == 0)                                                                                           //If the enmi is supposed to be a big one
    {
        if (army -> ennemies[index] -> active                                                                         //If the enemi is active
            && army -> ennemies[index] -> tag == E_SPAWN                                                              //And he is a spawnPoints
            && army -> ennemies[index] -> coolDown > (index / 7) + 5)                                                 //And his coolDown as reach a certein time
            {
                army -> ennemies[index] = gameobject_reset(E_FLOATING, 1,  army -> ennemies[index]);        //Initialize it to a Floating Mine
            }
    }
}

/*Create the enemies for the second level*/
void            spawn_lvl_two(int index, Army* army)
{
    if (index % 7 == 0)                                                                                           //If the enmi is supposed to be a big one
    {
        if (army -> ennemies[index] -> active                                                                         //If the enemi is active
            && army -> ennemies[index] -> tag == E_SPAWN                                                              //And he is a spawnPoints
            && army -> ennemies[index] -> coolDown > (index / 7) + 5)                                                 //And his coolDown as reach a certein time
            {
                army -> ennemies[index] = gameobject_reset(GetRandomValue(1, 2), 1,  army -> ennemies[index]);        //Initialize it to a Floating or Fireball Mine
            }
    }
}

/*Create the enemies for the third level*/
void            spawn_lvl_three(int index, Army* army)
{
    if (index % 7 == 0)                                                                                           //If the enmi is supposed to be a big one
    {
        if (army -> ennemies[index] -> active                                                                         //If the enemi is active
            && army -> ennemies[index] -> tag == E_SPAWN                                                              //And he is a spawnPoints
            && army -> ennemies[index] -> coolDown > (index / 7) + 5)                                                 //And his coolDown as reach a certein time
            {
                army -> ennemies[index] = gameobject_reset(GetRandomValue(1, 3), 1,  army -> ennemies[index]);        //Initialize it to a Floating or Fireball or Magnetic Mine
            }
    }
}

/*Create the enemies for the fourth level*/
void            spawn_lvl_four(int index, Army* army)
{
    if (index % 7 == 0)                                                                                           //If the enmi is supposed to be a big one
    {
        if (army -> ennemies[index] -> active                                                                         //If the enemi is active
            && army -> ennemies[index] -> tag == E_SPAWN                                                              //And he is a spawnPoints
            && army -> ennemies[index] -> coolDown > (index / 7) + 5)                                                 //And his coolDown as reach a certein time
            {
                army -> ennemies[index] = gameobject_reset(GetRandomValue(1, 4), 1,  army -> ennemies[index]);        //Initialize it to a Floating or Fireball or Magnetic or Magnetic-Fireball Mine
            }
    }
}

/*Initialize the GameObject 'SpawnPoint'*/
GameObject*     spawn_points_init(Vector2 center)
{
    GameObject* point = (GameObject*)malloc(sizeof(GameObject));                                //Stock the structure in the Heap memory and get a pointer to it
    if (point == NULL)                                                                          //If the pointer is NULL, we gob back and return the NULL value
        return NULL;

    point -> active = true;                                                                       //Set our point to active 
    point -> numberOfSides = 0;                                                                   //The spawn points has no sides because it's a circle
    point -> center = center;                                                                     //Set the point position to the given position
    point -> rotation = 0;                                                                        //Initialize the rotation to 0
    point -> life = 0;                                                                            //The spawPoint has no life because it's just a spawn points
    point -> speed = 0;                                                                           //Same but with speed
    point -> coolDown = 0;                                                                        //Initilize the Spawn Point coolDown to 0
    point -> tag = E_SPAWN;                                                                       //Set the right tag to our spawnPoint
    
    //Create different type of array that are in the Game Object struct and malloc them so we can use them later
    Vector2*  matrix = (Vector2*)malloc(sizeof(Vector2));
    float* angleArray = (float*)malloc(sizeof(float));
    unsigned int* radiusArray = (unsigned int*)malloc(sizeof(unsigned int));
    Vector2*  normalVec = (Vector2*)malloc(sizeof(Vector2)); 

    point -> radiusArray = radiusArray;                                                         
    point -> angleArray = angleArray;                                                            
    point -> vertexMatrix = matrix;  
    point -> normalVecArray = normalVec;

    return point;                                                                               //Returning the pointer to our Spawn Point
}

/*Reset a given GameObject to a 'SpawnPoint'*/
GameObject*     reset_to_spawnPoints(Type tag, GameObject* toReset)
{
    toReset -> numberOfSides = 0;                                             //The spawn points has no sides because it's a circle
    toReset -> rotation = 0;                                                  //Reset the rotation to 0
    toReset -> speed = 0;                                                     //The spawn points has no speed because he doesn't nove
    toReset -> tag = tag;                                                     //Reset the tag to the given tag
    toReset -> life = 0;                                                      //The spawn points has no life because he's just a spawn Point


    //Re alloc all the array from the GameObject Struct so the take less space in the heap memory
    toReset -> angleArray = realloc(toReset -> angleArray, sizeof(Vector2));
    toReset -> vertexMatrix = realloc(toReset -> vertexMatrix, sizeof(Vector2));
    toReset -> normalVecArray = realloc(toReset -> normalVecArray, sizeof(Vector2));
    toReset -> radiusArray = realloc(toReset -> radiusArray, sizeof(Vector2));

    //Set all the values of the arrray to NULL
    for (unsigned int y = 0; y < toReset -> numberOfSides; y++)
    {
        toReset -> angleArray[y] = 0;
        toReset -> radiusArray[y] = 0;
        toReset -> normalVecArray[y] = (Vector2){0};
        toReset -> vertexMatrix[y] = (Vector2){0};
    }

    return toReset;                                                         //Return the pointer to our Spawn Point
}

/*Reset a given GameObject to a 'fireballMine'*/
GameObject*     reset_to_fireballMine(Type tag, unsigned int life, GameObject* toReset)
{
    toReset -> numberOfSides = FIREBALL_MINE_SIDES;                                   //Reset the right number of sides                                  
    toReset -> rotation = GetRandomValue(1, 359);                                     //Get a random rotation so the enemi will go in a random direction
    toReset -> speed = FIREBALL_MINE_SPEED;                                           //Reset the right speed
    toReset -> tag = tag;                                                             //Reset the tag to the given tag
    toReset -> life = life;                                                           //Reset the life to the given life (For enemies life = size)

    //Realloc the different Arrays to the rright size so all the values are stocked at the right places
    toReset -> angleArray = realloc(toReset -> angleArray, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> vertexMatrix = realloc(toReset -> vertexMatrix, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> normalVecArray = realloc(toReset -> normalVecArray, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> radiusArray = realloc(toReset -> radiusArray, sizeof(Vector2) * toReset -> numberOfSides);


    for (unsigned int y = 0; y < toReset -> numberOfSides; y++)
    {
        toReset -> angleArray[y] = (360 / toReset -> numberOfSides) * y;                  //Reset the right angle in our angle array
        toReset -> radiusArray[y] = FIREBALL_MINE_RADIUS / life;                      //Reset the right radius in our radius array
    }

    for (unsigned int y = 1; y < toReset -> numberOfSides; y++)
    {
        toReset -> radiusArray[y] = (FIREBALL_MINE_RADIUS / life) / 2.20;               //One vertex out of two has a radius 2.20 times smaller
        y++;
    }

    update_vertex_array(toReset);
    update_vector_normal(toReset);

    return toReset;                                                                 //Return the pointer to our fireballeMine
}

/*Update function for the Fireball type*/
void            update_fireBall(Struct_list* list, GameObject* fireball)
{   
    if (!fireball -> active)                                                       //If the fireball is inactive, get out of the function
        return;

    update_vertex_array(fireball);                                              //Update the vertex array for the fireball
    update_vector_normal(fireball);                                             //Update the normal vector array fot he fireball

    //Moving the fireball forward 
    fireball -> center.x += cosf(DEG2RAD * fireball -> rotation) 
                        * FIREBALL_SPEED * list -> currentStatus -> deltaTime;
    fireball -> center.y += sinf(DEG2RAD * fireball -> rotation) 
                        * FIREBALL_SPEED * list -> currentStatus -> deltaTime;
}

/*Reset a given GameObject to a 'FireBall'*/
GameObject*     reset_to_fireball(unsigned int life, GameObject* toReset, GameObject* player)
{
    toReset -> numberOfSides = FIREBALL_NUMBER_SIDES;                                 //Reset the right number of sides                                  
    toReset -> speed = FIREBALL_SPEED;                                                //Reset the right speed
    toReset -> tag = E_FIREBALL;                                                      //Reset the tag to the given tag
    toReset -> life = life;                                                           //Reset the life to the given life (For enemies life = size)
    toReset -> coolDown = 0;                                                          //Reset the cooldown of the fireball

    Vector2 direction = {player -> center.x - toReset -> center.x, player -> center.y - toReset -> center.y };      //Calculate the distance between the player in the fireball
    toReset -> rotation = atan2f(direction.y, direction.x) * RAD2DEG;                                         //Orient the fireball to the player

    //Realloc the different Arrays to the rright size so all the values are stocked at the right places
    toReset -> angleArray = realloc(toReset -> angleArray, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> vertexMatrix = realloc(toReset -> vertexMatrix, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> normalVecArray = realloc(toReset -> normalVecArray, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> radiusArray = realloc(toReset -> radiusArray, sizeof(Vector2) * toReset -> numberOfSides);

    for (unsigned int y = 0; y < toReset -> numberOfSides; y++)
    {
        toReset -> angleArray[y] = (360 / toReset -> numberOfSides) * y;                  //Reset the angle array to the right values
        toReset -> radiusArray[y] = FIREBALL_RADIUS / life;                           //Reset the radius array to the right values
    }

    update_vertex_array(toReset);
    update_vector_normal(toReset);

    return toReset;                                                                 //Return the pointer to our fireBall
}

/*Reset a given GameObject to a 'FloatingMine'*/
GameObject*     reset_to_floatingMine(Type tag, unsigned int life, GameObject* toReset)
{
    toReset -> numberOfSides = FLOATING_MINE_SIDES;                                   //Reset the right number of sides                                  
    toReset -> rotation = GetRandomValue(1, 359);                                     //Get a random rotation so the enemi will go in a random direction
    toReset -> speed = FLOATING_MINE_SPEED;                                           //Reset the right speed
    toReset -> tag = tag;                                                             //Reset the tag to the given tag
    toReset -> life = life;                                                           //Reset the life to the given life (For enemies life = size)
    toReset -> coolDown = 0;

    //Realloc the different Arrays to the rright size so all the values are stocked at the right places
    toReset -> angleArray = realloc(toReset -> angleArray, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> vertexMatrix = realloc(toReset -> vertexMatrix, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> normalVecArray = realloc(toReset -> normalVecArray, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> radiusArray = realloc(toReset -> radiusArray, sizeof(Vector2) * toReset -> numberOfSides);

    for (unsigned int y = 0; y < toReset -> numberOfSides; y++)
    {
        toReset -> angleArray[y] = (360 / toReset -> numberOfSides) * y;                  //Reset the angle array the the right values
        toReset -> radiusArray[y] = (FLOATING_MINE_RADIUS / life);                    //Reset the radius array to the right values
    }

    for (unsigned int y = 1; y < toReset -> numberOfSides; y++)
    {
        toReset -> radiusArray[y] = (FLOATING_MINE_RADIUS / life) / 4;                //One vertex out of two has a radius 4 times smaller
        y++;
    }

    update_vertex_array(toReset);
    update_vector_normal(toReset);

    return toReset;                                                                 //Return a pointer to our FloatingMine
}

/*Reset a given GameObject to a 'MagneticFireballMine'*/
GameObject*     reset_to_magneticFireballMine(Type tag, unsigned int life, GameObject* toReset)
{
    toReset -> numberOfSides = MAGNETIC_FIREBALL_MINE_SIDES;                                      //Reset the right number of sides                                  
    toReset -> rotation = GetRandomValue(1, 359);                                                 //Get a random rotation so the enemi will go in a random direction
    toReset -> speed = MAGNETIC_FIREBALL_MINE_SPEED;                                              //Reset the right speed
    toReset -> tag = tag;                                                                         //Reset the tag to the given tag
    toReset -> life = life;                                                                       //Reset the life to the given life (For enemies life = size)
    toReset -> coolDown = 0;                                                                      //Reset the cooldown to 0

    //Realloc the different Arrays to the rright size so all the values are stocked at the right places
    toReset -> angleArray = realloc(toReset -> angleArray, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> vertexMatrix = realloc(toReset -> vertexMatrix, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> normalVecArray = realloc(toReset -> normalVecArray, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> radiusArray = realloc(toReset -> radiusArray, sizeof(Vector2) * toReset -> numberOfSides);

    for (unsigned int y = 0; y < toReset -> numberOfSides; y++)
    {
        toReset -> angleArray[y] = (360 / toReset -> numberOfSides) * y;                              //Reset the angle array to the right values
        toReset -> radiusArray[y] = (MAGNETIC_FIREBALL_MINE_RADIUS / life);                         //Reset the radius array to the right values
    }

    for (unsigned int y = 1; y < toReset -> numberOfSides; y++)
    {
        toReset -> radiusArray[y] = (MAGNETIC_FIREBALL_MINE_RADIUS / life) * 2;                       //One vertex out of two has a radius 2 times bigger    
        y++;
    }

    update_vertex_array(toReset);
    update_vector_normal(toReset);

    return toReset;
}

/*Reset a given GameObject to a 'MagneticMine'*/
GameObject*     reset_to_magneticMine(Type tag, unsigned int life, GameObject* toReset)
{
    toReset -> numberOfSides = MAGNETIC_MINE_SIDES;                                               //Reset the right number of sides                                  
    toReset -> rotation = GetRandomValue(1, 359);                                                 //Get a random rotation so the enemi will go in a random direction
    toReset -> speed = MAGNETIC_MINE_SPEED;                                                       //Reset the right speed
    toReset -> tag = tag;                                                                         //Reset the tag to the given tag
    toReset -> life = life;                                                                       //Reset the life to the given life (For enemies life = size)
    toReset -> coolDown = 0;                                                                      //Reset the cooldown to 0

    //Realloc the different Arrays to the rright size so all the values are stocked at the right places
    toReset -> angleArray = realloc(toReset -> angleArray, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> vertexMatrix = realloc(toReset -> vertexMatrix, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> normalVecArray = realloc(toReset -> normalVecArray, sizeof(Vector2) * toReset -> numberOfSides);
    toReset -> radiusArray = realloc(toReset -> radiusArray, sizeof(Vector2) * toReset -> numberOfSides);

    for (unsigned int y = 0; y < toReset -> numberOfSides; y++)
    {
        toReset -> angleArray[y] = (360 / toReset -> numberOfSides) * y;                              //Reset the angle array to the right values
        toReset -> radiusArray[y] = (MAGNETIC_MINE_RADIUS / life);                                  //Reset the radius array to the right values
    }

   for (unsigned int y = 1; y < toReset -> numberOfSides; y++)
    {
        toReset -> radiusArray[y] = (MAGNETIC_MINE_RADIUS / life) * 2.20;                                //One vertex out of two has a radius 3 times bigger    
        y++;
    }

    update_vertex_array(toReset);
    update_vector_normal(toReset);

    return toReset;
}

void            free_ennemies(Struct_list* list)
{
    for (unsigned int i = 0; i < list -> army -> nb_ennemies; i++)
    {
        free(list -> army -> ennemies[i] -> angleArray);
        free(list -> army -> ennemies[i] -> normalVecArray);
        free(list -> army -> ennemies[i] -> radiusArray);
        free(list -> army -> ennemies[i] -> vertexMatrix);
        free(list -> army -> ennemies[i]);
    }

    free(list -> army -> ennemies);
}