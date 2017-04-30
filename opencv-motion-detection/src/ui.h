#pragma once

#include <vector>

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include "imgui/gl3w.h"
#include <gl/GL.h>


namespace ui {
	SDL_Window* init();

	void render_window(SDL_Window *window);

	/**
	* Converts OpenCV Mat image format into OpenGL texture and returns texture ID
	**/
	GLuint mat_to_tex(const cv::Mat& mat);

	void cleanup(SDL_Window *window);

	// Event handling
	enum mouse_buttons
	{
		LEFT = 0,
		MIDDLE = 1,
		RIGHT = 2
	};

	extern std::vector<bool> mouse_button_states;
	extern cv::Point mouse_pos;

	void on_mouse_motion(SDL_Event& event);
	void on_mouse_button_down(SDL_Event& event);
	void on_mouse_button_up(SDL_Event& event);
}