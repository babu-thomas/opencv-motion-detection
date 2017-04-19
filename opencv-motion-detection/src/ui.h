#pragma once

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include "imgui/gl3w.h"
#include <gl/GL.h>

SDL_Window* ui_init();

void render_window(SDL_Window *window);

/**
 * Converts OpenCV Mat image format into OpenGL texture and returns texture ID
 **/
GLuint mat_to_tex(const cv::Mat& mat);

void ui_cleanup(SDL_Window *window);