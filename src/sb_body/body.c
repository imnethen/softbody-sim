#include "body.h"
#include <stdlib.h>

// inits (app->num_points) points uniformally on a circle
void InitPointsCircle(SB_Body *body, Vector center, float radius) {
    const float tau = 6.2831853;
    for (int i = 0; i < body->num_points; i++) {
        float angle = tau * ((float)i / (float)(body->num_points));
        float x = SDL_cosf(angle) * radius + center.x;
        float y = SDL_sinf(angle) * radius + center.y;
        body->points[i] = SB_CreatePoint(x, y);
    }
}

void AllocAndInitConstraintsCircle(SB_Body *body, float radius) {
    const float tau = 6.2831853;
    body->num_constraints = body->num_points;
    body->constraints = malloc(sizeof(SB_Constraint) * body->num_constraints);

    float constraint_length = radius * SDL_sqrtf(2. * (1. - SDL_cosf(tau / (float)body->num_points)));

    for (int i = 0; i < body->num_constraints; i++) {
        body->constraints[i] = (SB_Constraint) { &body->points[i], &body->points[(i + 1) % body->num_points], constraint_length };
    }
}

SB_Body SB_CreateBodyCircle(int num_points, Vector center, float radius) {
    SB_Body body;
    body.num_points = num_points;
    body.num_constraints = num_points;

    body.points = malloc(sizeof(SB_Point) * body.num_points);
    InitPointsCircle(&body, center, radius);
    AllocAndInitConstraintsCircle(&body, radius);

    return body;
}

void SB_UpdateBody(SB_Body *body, float deltatime) {
    for (int i = 0; i < body->num_points; i++) {
        SB_UpdatePoint(&body->points[i], deltatime);
    }

    for (int i = 0; i < body->num_constraints; i++) {
        SB_UpdateConstraint(&body->constraints[i]);
    }
}

void SB_RenderBody(SB_Body *body, SDL_Renderer *renderer) {
    for (int i = 0; i < body->num_points; i++) {
        SB_RenderPoint(&body->points[i], renderer);
    }

    for (int i = 0; i < body->num_constraints; i++) {
        SB_RenderConstraint(&body->constraints[i], renderer);
    }
}
