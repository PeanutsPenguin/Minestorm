#pragma once
#include "game_manager.h"

typedef struct struct_list_s Struct_list;
typedef struct GameObject_s GameObject;
typedef struct player_array_s Player_array;
typedef struct game_status_s Game_status;
typedef struct army_s Army;


//Define for Player
#define PLAYER_SPEED 500
#define PLAYER_ROTATION_SPEED 400
#define PLAYER_LIFE 3
#define PLAYER_NUMBER_SIDES 4
#define PLAYER_RADIUS 20

//Define projectile
#define PROJECTILE_NUMBER_SIDES 8
#define PROJECTILE_LIFE 1
#define PROJECTILE_SPEED 800
#define PROJECTILE_RADIUS 4

Player_array*   player_array_init(unsigned int nbrOfProjectile);

GameObject*     player_Init(Vector2 position);

GameObject*     projectile_init();

void            update_playerOne(Struct_list* list);

void            control_player_one(Struct_list* list);

void            check_collision_enemis_projectile(GameObject* projectile, Army* enemi, GameObject* player, Struct_list* list);

void            check_collision_enemis_player(Struct_list* list, GameObject* player);

void            projectile_create(GameObject** projectile_array, unsigned int nbrOfProjectile, GameObject* Player);

void            update_playerTwo(Struct_list* list);

void            control_player_two(Struct_list* list);

void            projectile_create_two(Struct_list* list_of_struct);

void            projectile_update(GameObject* projectile, Game_status* currentStatus);

Vector2         player_teleportation(GameObject* player);

void            free_playerOne(Struct_list* list);

void            free_playerTwo(Struct_list* list);