#pragma once 
#include "game_manager.h"

typedef struct GameObject_s GameObject;
typedef struct game_status_s Game_status;
typedef struct struct_list_s Struct_list;
typedef struct army_s Army;

typedef enum type_e Type;

//Floating Mine
#define FLOATING_MINE_SPEED 100
#define FLOATING_MINE_SIDES 6
#define FLOATING_MINE_RADIUS 40

//Fireball Mine
#define FIREBALL_MINE_SPEED 100
#define FIREBALL_MINE_SIDES 8
#define FIREBALL_MINE_RADIUS 40

//Magnetic Mine
#define MAGNETIC_MINE_SIDES 8
#define MAGNETIC_MINE_SPEED 100
#define MAGNETIC_MINE_RADIUS 18

//Magnetic Fireball Mine
#define MAGNETIC_FIREBALL_MINE_SIDES 10
#define MAGNETIC_FIREBALL_MINE_SPEED 100
#define MAGNETIC_FIREBALL_MINE_RADIUS 25

//Fireball
#define FIREBALL_SPEED 200
#define FIREBALL_NUMBER_SIDES 12
#define FIREBALL_RADIUS 8

//Spawn points
#define POINT_RADIUS 5

//Spawn the right enemies by level
void            spawn_lvl_one(int index, Army* army);
void            spawn_lvl_two(int index, Army* army);
void            spawn_lvl_three(int index, Army* army);
void            spawn_lvl_four(int index, Army* army);

//Initialize spawn points
GameObject*     spawn_points_init(Vector2 center);

//Update enemies 
void            update_ennemies(Struct_list* list);
void            update_basic_mine(GameObject* mine, Game_status* status);
void            update_magnetic_mine(GameObject* mine, Struct_list* list);
void            update_fireBall(Struct_list* list, GameObject* fireball);

//Reset enemies
void            reset_enemies(Struct_list* list);
GameObject*     reset_to_spawnPoints(Type tag, GameObject* toReset);
GameObject*     reset_to_fireballMine(Type tag, unsigned int life, GameObject* toReset);
GameObject*     reset_to_fireball(unsigned int life, GameObject* toReset, GameObject* player);
GameObject*     reset_to_floatingMine(Type tag, unsigned int life, GameObject* toReset);
GameObject*     reset_to_magneticFireballMine(Type tag, unsigned int life, GameObject* toReset);
GameObject*     reset_to_magneticMine(Type tag, unsigned int life, GameObject* toReset);

void            free_ennemies(Struct_list* list);