#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <SDL.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"
#include "utils.h"
#include "filters.h"
#include "ui.h"

#undef main

int main(int argc, char *argv[])
{
	SDL_Window *window = ui_init();
	cv::VideoCapture video;
	bool success, first_run = true;
	cv::Mat src_frame, motion_frame;
	GLuint src_frame_tex, motion_frame_tex;

	auto bg_model = cv::createBackgroundSubtractorMOG2();

	Uint32 old_time = SDL_GetTicks();
	Uint32 new_time;
	float seconds_passed;
	float FPS = 25;

	bool done = false;
	while (!done)
	{
		// Handle Events
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
		}

		new_time = SDL_GetTicks();
		seconds_passed = (float)(new_time - old_time) / 1000;

		if (seconds_passed > (1 / FPS))
		{
			ImGui_ImplSdlGL3_NewFrame(window);
			ImGui::SetNextWindowSize(ImVec2(1280, 720));
			ImGui::Begin("Main Window", (bool*)true, ImGuiWindowFlags_AlwaysAutoResize
				| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove);
			ImGui::SetWindowFontScale(1.5f);
			static char video_path[128] = "";
			ImGui::Text("Input Video Path: ");
			ImGui::SameLine();
			ImGui::PushItemWidth(800);
			ImGui::InputText("##", video_path, 128);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("OK", ImVec2(45, 25)))
			{
				bool opened = video.open(video_path);
				if (!opened)
					std::cout << "Can't open video from provided path!! Try again with correct path."
					<< std::endl;
			}
			if (video.isOpened())
			{
				success = video.read(src_frame);
				if (!success)
				{
					std::cout << "Can't read video frame!!";
					break;
				}
				if (!first_run)
				{
					glDeleteTextures(1, &src_frame_tex);
					glDeleteTextures(1, &motion_frame_tex);
				}
				cv::Mat src_frame_gray = cvt_color(src_frame, cv::COLOR_BGR2GRAY);
				cv::Mat mask = mog2(src_frame_gray, bg_model);
				mask = dilate(mask, 3);
				auto contours = find_contours(mask);

				src_frame.copyTo(motion_frame);
				cv::Rect box;
				for (auto& c : contours)
				{
					if (cv::contourArea(c) < 1000 || cv::contourArea(c) > 5000)
						continue;
					box = cv::boundingRect(c);
					cv::rectangle(motion_frame, box, cv::Scalar(0, 0, 255));
				}

				float video_feed_width = ImGui::GetWindowContentRegionWidth() * 0.5f;
				float video_feed_height = ImGui::GetWindowHeight() - 50;
				src_frame = resize(src_frame, video_feed_width, 0, cv::INTER_LINEAR);
				src_frame_tex = mat_to_tex(src_frame);
				motion_frame = resize(motion_frame, video_feed_width, 0, cv::INTER_CUBIC);
				motion_frame_tex = mat_to_tex(motion_frame);
				first_run = false;

				ImGui::BeginChild("Video Feed", ImVec2(video_feed_width, video_feed_height), false);
				ImGui::Image((void*)src_frame_tex, ImVec2(src_frame.size().width,
					src_frame.size().height));
				ImGui::SetWindowFontScale(1.5f);
				ImGui::Text("SOURCE VIDEO");
				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("Motion Detection", ImVec2(video_feed_width, video_feed_height),
					false);
				ImGui::Image((void*)motion_frame_tex, ImVec2(motion_frame.size().width,
					motion_frame.size().height));
				ImGui::SetWindowFontScale(1.5f);
				ImGui::Text("RESULT OF MOTION DETECTION");
				ImGui::EndChild();
			}
			ImGui::End();
			render_window(window);
			old_time = new_time;
		}
	}
	ui_cleanup(window);
	return 0;
}