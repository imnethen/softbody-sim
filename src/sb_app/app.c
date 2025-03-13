#include "../sb_body/body.h"
#include "app.h"
#include <stdlib.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_stdinc.h>

typedef struct SB_App {
    int last_update_ns;

    int num_points;
    SB_Point *points;
    int num_constraints;
    SB_Constraint *constraints;
} SB_App;

// only used once in init so that points dont go flying at the start
void UpdateConstraintsWithoutVelocityUpdates(SB_App *app) {
    for (int i = 0; i < app->num_constraints; i++) {
        SB_UpdateConstraint(&app->constraints[i]);
    }
    for (int i = 0; i < app->num_points; i++) {
        app->points[i].last_pos = app->points[i].pos;
    }
}

// inits (app->num_points) points uniformally on a circle
void InitPointsCircle(SB_App *app, Vector center, float radius) {
    const float tau = 6.2831853;
    for (int i = 0; i < app->num_points; i++) {
        float angle = tau * ((float)i / (float)(app->num_points));
        float x = SDL_cosf(angle) * radius + center.x;
        float y = SDL_sinf(angle) * radius + center.y;
        app->points[i] = SB_CreatePoint(x, y);
    }
}

void AllocAndInitConstraintsCircle(SB_App *app, float radius) {
    const float tau = 6.2831853;
    app->num_constraints = app->num_points;
    app->constraints = malloc(sizeof(SB_Constraint) * app->num_constraints);

    float constraint_length = radius * SDL_sqrtf(2. * (1. - SDL_cosf(tau / (float)app->num_points)));

    for (int i = 0; i < app->num_constraints; i++) {
        app->constraints[i] = (SB_Constraint) { &app->points[i], &app->points[(i + 1) % app->num_points], constraint_length };
    }
}

bool SB_InitApp(SB_App **app) {
    *app = malloc(sizeof(SB_App));

    (*app)->last_update_ns = SDL_GetTicksNS();

    (*app)->num_points = 60;
    (*app)->points = malloc(sizeof(SB_Point) * (*app)->num_points);
    InitPointsCircle(*app, (Vector){ 500., 500. }, 200.);
    AllocAndInitConstraintsCircle(*app, 200.);

    /* UpdateConstraintsWithoutVelocityUpdates(*app); */

    return true;
}

bool SB_AppHandleEvent(SB_App *app, SDL_Event *event) {

    return true;
}

bool SB_UpdateApp(SB_App *app, int cur_time_ns) {
    float deltatime = (float)(cur_time_ns - app->last_update_ns) / 1e6;
    app->last_update_ns = cur_time_ns;

    for (int i = 0; i < app->num_points; i++) {
        SB_UpdatePoint(&app->points[i], deltatime);
    }

    for (int i = 0; i < app->num_constraints; i++) {
        SB_UpdateConstraint(&app->constraints[i]);
    }

    return true;
}

bool SB_RenderApp(SB_App *app, SDL_Renderer *renderer) {
    for (int i = 0; i < app->num_points; i++) {
        SB_RenderPoint(&app->points[i], renderer);
    }

    for (int i = 0; i < app->num_constraints; i++) {
        SB_RenderConstraint(&app->constraints[i], renderer);
    }

    return true;
}
