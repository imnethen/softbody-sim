#include "blob.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

SB_Point SB_CreatePoint(float x, float y) {
    return (SB_Point) {
        (Vector){ x, y },
        (Vector){ x, y },
        (Vector){ 0., 0. },
    };
}

SB_Point SB_CreatePointVec(Vector pos) {
    return (SB_Point) {
        pos,
        pos,
        (Vector){ 0., 0. },
    };
}

void SB_PointHandleBorders(SB_Point *point) {
    const float lower_border = 1080.;
    const float right_border = 1920.;

    if (point->pos.y > lower_border) {
        point->pos.y = lower_border;
    }
    if (point->pos.x > right_border) {
        point->pos.x = right_border;
    }
    if (point->pos.x < 0.) {
        point->pos.x = 0.;
    }
}

bool SB_UpdatePoint(SB_Point *point, float deltatime) {
    const float gravity = 0.02;
    Vector start_pos = point->pos;

    point->acc = (Vector){ 0., 0. };
    point->acc = VectorAdd(point->acc, (Vector){ 0., gravity });

    Vector acc_dt = VectorMul(point->acc, deltatime * deltatime);
    Vector vel = VectorSub(point->pos, point->last_pos);
    vel = VectorMul(vel, 0.99);
    point->pos = VectorAdd(point->pos, VectorAdd(vel, acc_dt));

    SB_PointHandleBorders(point);

    point->last_pos = start_pos;
    return true;
}

bool SB_RenderPoint(SB_Point *point, SDL_Renderer *renderer) {
    const int point_size = 10;

    SDL_RenderPoint(renderer, point->pos.x, point->pos.y);
    SDL_FRect rect = (SDL_FRect){ point->pos.x - point_size/2, point->pos.y - point_size/2, point_size, point_size };
    SDL_RenderFillRect(renderer, &rect);

    return true;
}
