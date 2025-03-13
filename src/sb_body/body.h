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


typedef struct SB_Body {
    int num_points;
    SB_Point *points;
    int num_constraints;
    SB_Constraint *constraints;
} SB_Body;

SB_Body SB_CreateBodyCircle(int num_points, Vector center, float radius);
void SB_UpdateBody(SB_Body *body, float deltatime);
void SB_RenderBody(SB_Body *body, SDL_Renderer *renderer);
