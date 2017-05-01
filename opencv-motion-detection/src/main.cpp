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

int main(int argc, char *argv[])
{
	SDL_Window *window = ui::init();
	cv::VideoCapture video;
	bool success, first_run = true, draw_crop = false;
	cv::Mat src_frame, motion_frame;
	GLuint src_frame_tex, motion_frame_tex, crop_roi_tex;
	cv::Point rect_pt1, rect_pt2;
	cv::Rect selection;
	cv::Mat crop_roi;
	bool dragging = false;

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
			switch (event.type)
			{
			case SDL_QUIT:
				done = true;
				break;
			case SDL_MOUSEMOTION:
				ui::on_mouse_motion(event);
				if(dragging)
					rect_pt2 = ui::mouse_pos;
				break;
			case SDL_MOUSEBUTTONDOWN:
				ui::on_mouse_button_down(event);
				if (ui::mouse_button_states[0])
				{
					dragging = true;
					rect_pt1 = ui::mouse_pos;
					rect_pt2 = ui::mouse_pos;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				ui::on_mouse_button_up(event);
				if (!ui::mouse_button_states[0])
				{
					dragging = false;
					rect_pt2 = ui::mouse_pos;
				}
				break;
			default:
				break;
			}
		}

		new_time = SDL_GetTicks();
		seconds_passed = (float)(new_time - old_time) / 1000;

		if (seconds_passed > (1 / FPS))
		{
			// Background Window
			ImGui_ImplSdlGL3_NewFrame(window);
			ImGui::SetNextWindowSize(ImVec2(1280, 720));
			ImGui::Begin("Main Window", (bool*)true, ImGuiWindowFlags_AlwaysAutoResize
				| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove);

			// Textbox Label
			ImGui::SetWindowFontScale(1.5f);
			static char video_path[128] = "";
			ImGui::Text("Input Video Path: ");

			// Textbox
			ImGui::SameLine();
			ImGui::PushItemWidth(800);
			ImGui::InputText("##", video_path, 128);
			ImGui::PopItemWidth();

			// OK Button
			ImGui::SameLine();
			if (ImGui::Button("OK", ImVec2(45, 25)))
			{
				bool opened = video.open(video_path);
				if (!opened)
					std::cout << "Can't open video from provided path!! Try again with correct path."
					<< std::endl;
			}

			ImGui::ShowTestWindow();

			if (video.isOpened())
			{
				success = video.read(src_frame);
				if (!success)
				{
					std::cout << "Can't read video frame!!";
					break;
				}

				// Delete textures from previous frame
				if (!first_run)
				{
					glDeleteTextures(1, &src_frame_tex);
					glDeleteTextures(1, &crop_roi_tex);
				}

				// Apply filters
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
				src_frame = resize(src_frame, video_feed_width, 0, cv::INTER_CUBIC);

				int win_pos_x = int(ImGui::GetCursorPosX() + 0.5f);
				int win_pos_y = int(ImGui::GetCursorPosY() + 0.5f);
				cv::Point win_pos(win_pos_x, win_pos_y);

				// Draw selection
				if (dragging)
				{
					selection = { rect_pt1 - win_pos, rect_pt2 - win_pos };
				}
				cv::rectangle(src_frame, selection, cv::Scalar(0, 255, 0), 2);
				crop_roi = src_frame(selection);
				if (crop_roi.size().width > 20 && crop_roi.size().height > 20)
					draw_crop = true;

				// Video Feed Window
				ImGui::BeginChild("Video Feed", ImVec2(video_feed_width, video_feed_height));
				src_frame_tex = ui::mat_to_tex(src_frame);
				ImGui::Image((void*)src_frame_tex, ImVec2(src_frame.size().width,
					src_frame.size().height));
				ImGui::EndChild();

				// Cropped Feed Window
				ImGui::SameLine();
				ImGui::BeginChild("Cropped Feed", ImVec2(video_feed_width, video_feed_height));
				ImGui::SetWindowFontScale(1.5f);
				ImGui::Text("Motion in Selected Region");
				if (draw_crop)
				{
					cv::Mat crop_roi_rsz = resize(crop_roi, video_feed_width, 0, cv::INTER_CUBIC);
					crop_roi_rsz = resize(crop_roi_rsz, video_feed_width, 0, cv::INTER_CUBIC);
					crop_roi_tex = ui::mat_to_tex(crop_roi_rsz);
					ImGui::Image((void*)crop_roi_tex, ImVec2(crop_roi.size().width,
						crop_roi.size().height));
					draw_crop = false;
				}
				ImGui::EndChild();

				first_run = false;
			}
			ImGui::End();
			ui::render_window(window);
			old_time = new_time;
		}
	}
	ui::cleanup(window);
	return 0;
}

