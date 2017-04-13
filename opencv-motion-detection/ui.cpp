#include <iostream>
#include "imgui/gl3w.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

#include "ui.h"

SDL_GLContext glcontext;

SDL_Window* ui_init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		std::cout << "Can't Initialize SDL. Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	SDL_Window *window = SDL_CreateWindow("Motion Detection", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	glcontext = SDL_GL_CreateContext(window);
	gl3wInit();
	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(window);

	return window;
}

void render_window(SDL_Window* window)
{
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
	SDL_GL_SwapWindow(window);
}

void ui_cleanup(SDL_Window *window)
{
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
