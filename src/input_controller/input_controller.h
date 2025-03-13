#pragma once
#include <SDL3/SDL_events.h>
#include "../vector/vector.h"


// mouse only
typedef struct InputController {
    bool left_mouse_pressed;
    Vector mouse_pos;
} InputController;

void IC_HandleEvent(InputController* input_controller, SDL_Event *event);
