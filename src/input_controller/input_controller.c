#include "input_controller.h"
#include <SDL3/SDL_events.h>


void IC_HandleEvent(InputController* input_controller, SDL_Event *event) {
     if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        input_controller->left_mouse_pressed = true;
     } else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        input_controller->left_mouse_pressed = false;
     } else if (event->type == SDL_EVENT_MOUSE_MOTION) {
        input_controller->mouse_pos = (Vector) { event->motion.x, event->motion.y };
     }
}
