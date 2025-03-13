#include "app.h"
#include "../vector/vector.h"
#include "../sb_blob/blob.h"
#include "../input_controller/input_controller.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_stdinc.h>

typedef struct SB_App {
    int last_update_ns;
    InputController input_controller;

    SB_Blob blob;
} SB_App;

bool SB_InitApp(SB_App **app) {
    *app = malloc(sizeof(SB_App));

    (*app)->last_update_ns = SDL_GetTicksNS();
    (*app)->input_controller.left_mouse_pressed = false;
    (*app)->blob = SB_CreateBlob(60, (Vector){ 500., 500. }, 200., 125e3, 0.001);

    return true;
}

bool SB_UpdateApp(SB_App *app, int cur_time_ns) {
    float deltatime = (float)(cur_time_ns - app->last_update_ns) / 1e6;
    app->last_update_ns = cur_time_ns;

    if (app->input_controller.left_mouse_pressed) {
        int closest_index = 0;
        float closest_dist = 1e9;

        for (int i = 0; i < app->blob.num_points; i++) {
            Vector pos = app->blob.points[i].pos;
            float dist = VectorLen(VectorSub(pos, app->input_controller.mouse_pos));
            /* printf("%f %f; %f %f; %f\n", pos.x, pos.y, app->input_controller.mouse_pos.x, app->input_controller.mouse_pos.y, dist); */
            if (dist < closest_dist) {
                closest_dist = dist;
                closest_index = i;
            }
        }

        app->blob.points[closest_index].pos = app->input_controller.mouse_pos;
    }

    SB_UpdateBlob(&app->blob, deltatime);

    return true;
}

bool SB_RenderApp(SB_App *app, SDL_Renderer *renderer) {
    SB_RenderBlob(app->blob, renderer);

    return true;
}

bool SB_AppHandleEvent(SB_App *app, SDL_Event *event) {
    IC_HandleEvent(&app->input_controller, event);

    return true;
}
