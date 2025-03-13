#include "app.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

Point SB_CreatePoint(float x, float y) {
    return (Point) {
        (Vector){ x, y },
        (Vector){ x, y },
        (Vector){ 0., 0. },
    };
}

Point SB_CreatePointVec(Vector pos) {
    return (Point) {
        pos,
        pos,
        (Vector){ 0., 0. },
    };
}

void handle_borders(Point *point) {
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

bool SB_UpdatePoint(Point *point, float deltatime) {
    const float gravity = 0.02;
    Vector start_pos = point->pos;

    point->acc = (Vector){ 0., 0. };
    point->acc = VectorAdd(point->acc, (Vector){ 0., gravity });

    Vector acc_dt = VectorMul(point->acc, deltatime * deltatime);
    Vector vel = VectorSub(point->pos, point->last_pos);
    vel = VectorMul(vel, 0.99);
    point->pos = VectorAdd(point->pos, VectorAdd(vel, acc_dt));

    handle_borders(point);

    point->last_pos = start_pos;
    return true;
}

bool SB_RenderPoint(Point *point, SDL_Renderer *renderer) {
    const int point_size = 10;

    SDL_RenderPoint(renderer, point->pos.x, point->pos.y);
    SDL_FRect rect = (SDL_FRect){ point->pos.x - point_size/2, point->pos.y - point_size/2, point_size, point_size };
    SDL_RenderFillRect(renderer, &rect);

    return true;
}
