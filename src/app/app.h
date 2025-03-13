#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_events.h>
#include "../vector/vector.h"


typedef struct SB_App SB_App;

bool SB_InitApp(SB_App **app);
bool SB_UpdateApp(SB_App *app, int cur_time_ms);
bool SB_RenderApp(SB_App *app, SDL_Renderer *renderer);
bool SB_AppHandleEvent(SB_App *app, SDL_Event *event);


typedef struct Point {
    Vector pos;
    Vector last_pos;
    Vector acc;
} Point;

Point SB_CreatePoint(float x, float y);
Point SB_CreatePointVector(Vector pos);
bool SB_UpdatePoint(Point *point, float deltatime);
bool SB_RenderPoint(Point *point, SDL_Renderer *renderer);


typedef struct Constraint {
    Point *pointA;
    Point *pointB;
    float length;
} Constraint;

bool SB_UpdateConstraint(Constraint *constraint);
bool SB_RenderConstraint(Constraint *constraint, SDL_Renderer *renderer);
