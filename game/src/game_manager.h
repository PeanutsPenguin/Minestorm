#pragma once
//Libraries include
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include <stdbool.h>

//Files include
#include "player.h"
#include "math_function.h"
#include "enemy.h"
#include "hud.h"

//Screen define
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 1200

//Define two*PI si it's easier to understand in the code
#define TWOPI 2*PI

//Enumaration for the different screens possible in game 
typedef enum screen_e
{
    E_TITLE,
    E_LVLONE,
    E_LVLTWO,
    E_LVLTHREE,
    E_LVLFOUR,
    E_GAMEOVER
}Screen;

//Enumaration of the different type of GameObject possible in the game
typedef enum type_e
{
    E_SPAWN,
    E_FLOATING,
    E_FIREBALL_MINE, 
    E_MAGNETIC, 
    E_MAGNETIC_FIREBALL,
    E_FIREBALL,
    E_PLAYER,
    E_PLAYER_PROJECTILE,
}Type;

//Structure of all the object in the Game 
typedef struct GameObject_s
{
    Vector2* vertexMatrix;          //Array of vector2 that stocks all the vertexes of the object
    Vector2* normalVecArray;        //Array of vector2 that stocks all the normal vector of the object, very useful for the SAT collisions
    float* angleArray;              //Array of float that stock the angle (in a trigonometric circle) between x axis and the vertexes, useful for rotation
    unsigned int* radiusArray;      //Array of int that stocks the radius of the different vertexes of the object
    Vector2 center;                 //This is the center of the object
    float speed;                    //This is the speed of the object
    float rotation;                 //This is the roation of the object
    float coolDown;                 //This is the cooldown of the object, useful for power or spawn
    unsigned int life;              //This is the life of the object
    unsigned int numberOfSides;     //This is the number of sides of the object
    Type tag;                       //This is the tag of the object, useful to know wich object is it
    bool active;                    //This a boolean to know if it's alive or not
}GameObject;

//Structure that contains the players
typedef struct player_array_s
{
    GameObject** projectileArrayOne;        //Projectile array for the player one
    GameObject** projectileArrayTwo;        //Projectile array for the player two
    GameObject* playerOne;                  //Player one
    GameObject* playerTwo;                  //Player two
    unsigned int nbrOfProjectile;           //Number of projectile for both player
}Player_array;

//Structure that contains all the enemies 
typedef struct army_s
{
    GameObject** ennemies;      //Array of enemy
    int nb_ennemies;            //Number of enemies
    int enemy_left;             //Number of enemy left
}Army;

//Structure that contains different status in the game
typedef struct game_status_s
{
    int score;
    float deltaTime;                //Float to have the time in seconds
    Screen currentScreen;           //Enmuaration to know what is the current screen
    bool isGamePaused;              //Boolean to know if the game is paused or not
    bool isScreenSwitching;         //Boolean to know if we are switching screen or not
    bool bothPlayer;                //Boolean to know if there is one or two player
}Game_status;

//Structure that contains pointers to all the struct above
typedef struct struct_list_s
{
    Player_array* player;
    Game_status*  currentStatus;
    Army* army;
}Struct_list;

Game_status*    game_status_init();

Army*           army_init(int nb_ennemies);

Struct_list*    struct_list_init(Player_array* player, Game_status* currentStatus, Army* army);

void            update_and_draw(Struct_list* list_of_struct);

void            draw_game(Struct_list* list_of_struct);

void            draw_GameObject(GameObject* object, Color color);

void            draw_title_screen(Struct_list* list);

void            draw_game_over_screen(Struct_list* list);

void            draw_switch_screen(Struct_list* list);

void            update_game(Struct_list* list_of_struct);

void            update_title_screen(Struct_list* list);

void            update_game_over_screen(Struct_list* list);

void            switch_level(Struct_list* list, Screen nextLevel);

GameObject*     gameobject_reset(Type tag, unsigned int life, GameObject* toReset);

void free_game(Struct_list* list);