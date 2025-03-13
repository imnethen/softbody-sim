#pragma once

typedef struct SB_App SB_App;

bool SB_InitApp(SB_App **app);
bool SB_UpdateApp(SB_App *app, int cur_time_ms);
bool SB_RenderApp(SB_App *app, SDL_Renderer *renderer);
bool SB_AppHandleEvent(SB_App *app, SDL_Event *event);
