#pragma once

#include <SDL.h>

SDL_Window* ui_init();

void render_window(SDL_Window *window);
void ui_cleanup(SDL_Window *window);