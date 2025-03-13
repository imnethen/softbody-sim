#include "body.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

bool SB_UpdateConstraint(Constraint *constraint) {
    Vector posA = constraint->pointA->pos;
    Vector posB = constraint->pointB->pos;

    Vector center = VectorDiv(VectorAdd(posA, posB), 2.);

    Vector center_to_a = VectorSub(posA, center);
    if (VectorLenSq(center_to_a) == 0.) {
        return true;
    }

    Vector center_to_newa = VectorMul(VectorNormalized(center_to_a), constraint->length / 2.);
    Vector newA = VectorAdd(center, center_to_newa);

    Vector center_to_b = VectorSub(posB, center);
    if (VectorLenSq(center_to_b) == 0.) {
        return true;
    }

    Vector center_to_newb = VectorMul(VectorNormalized(center_to_b), constraint->length / 2.);
    Vector newB = VectorAdd(center, center_to_newb);

    constraint->pointA->pos = newA;
    constraint->pointB->pos = newB;

    return true;
}

bool SB_RenderConstraint(Constraint *constraint, SDL_Renderer *renderer) {
    float xa = constraint->pointA->pos.x;
    float ya = constraint->pointA->pos.y;
    float xb = constraint->pointB->pos.x;
    float yb = constraint->pointB->pos.y;

    SDL_RenderLine(renderer, xa, ya, xb, yb);

    return true;
}
