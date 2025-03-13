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
void SB_PointHandleBorders(SB_Point *point);


typedef struct SB_Blob {
    int num_points;
    SB_Point *points;
    float constraint_length;
    float desired_volume;
    float scaling_factor;
} SB_Blob;

SB_Blob SB_CreateBlob(int num_points, Vector center, float radius, float desired_volume, float scaling_factor);
void SB_UpdateBlob(SB_Blob *blob, float deltatime);
void SB_RenderBlob(SB_Blob blob, SDL_Renderer *renderer);
