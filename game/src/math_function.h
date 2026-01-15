#pragma once
#include "game_manager.h"

typedef struct GameObject_s GameObject;

Vector2         vector_between_summits(Vector2 firstSummit, Vector2 secondSummit);

float           vector_norm(Vector2 toCvonvert);

Vector2         vector_normal(Vector2 toNormalize);

void            update_vertex_array(GameObject* object);

void            update_vector_normal(GameObject* object);

bool            SAT_between_form(GameObject* formOne, GameObject* formTwo);

bool            check_max_and_min(Vector2 normedVector, Vector2* vertexArrayOne, Vector2* vertexArrayTwo, unsigned int nbrOfVErtexOne, unsigned int nbrOfVertexTwo);

bool            SAT_between_convexe_and_concave(GameObject* formOne, GameObject* formTwo);

Vector2         move_to_vector(Vector2 vecToMove, Vector2 direction, float speed);

void            border_collision (GameObject* entity);

void            stable_rotation(GameObject* entity);