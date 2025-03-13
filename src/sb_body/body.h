#pragma once
#include <SDL3/SDL_render.h>
#include "../vector/vector.h"


typedef struct SB_Point {
    Vector pos;
    Vector last_pos;
    Vector acc;
} SB_Point;

SB_Point SB_CreatePoint(float x, float y);
SB_Point SB_CreatePointVector(Vector pos);
bool SB_UpdatePoint(SB_Point *point, float deltatime);
bool SB_RenderPoint(SB_Point *point, SDL_Renderer *renderer);


typedef struct SB_Constraint {
    SB_Point *pointA;
    SB_Point *pointB;
    float length;
} SB_Constraint;

bool SB_UpdateConstraint(SB_Constraint *constraint);
bool SB_RenderConstraint(SB_Constraint *constraint, SDL_Renderer *renderer);
