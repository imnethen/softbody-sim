#include "app.h"
#include "../vector/vector.h"
#include "../sb_body/body.h"
#include <stdlib.h>
#include <SDL3/SDL_timer.h>

typedef struct SB_App {
    int last_update_ns;
    SB_Body body;
} SB_App;

bool SB_InitApp(SB_App **app) {
    *app = malloc(sizeof(SB_App));

    (*app)->last_update_ns = SDL_GetTicksNS();

    (*app) -> body = SB_CreateBodyCircle(60, (Vector){ 500., 500. }, 200.);

    return true;
}

bool SB_AppHandleEvent(SB_App *app, SDL_Event *event) {

    return true;
}

bool SB_UpdateApp(SB_App *app, int cur_time_ns) {
    float deltatime = (float)(cur_time_ns - app->last_update_ns) / 1e6;
    app->last_update_ns = cur_time_ns;

    SB_UpdateBody(&app->body, deltatime);

    return true;
}

bool SB_RenderApp(SB_App *app, SDL_Renderer *renderer) {
    SB_RenderBody(&app->body, renderer);

    return true;
}
