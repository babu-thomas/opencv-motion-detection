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

	cv::Mat frame = cv::imread("snoop_dogg.jpg");
	GLuint snoop_tex = mat_to_tex(frame);

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

		ImGui_ImplSdlGL3_NewFrame(window);
		ImGui::ShowTestWindow((bool*)true);
		ImGui::SetNextWindowSize(ImVec2(1280, 720));
		ImGui::Begin("Main Window", (bool*)true, ImGuiWindowFlags_AlwaysAutoResize
			| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowFontScale(1.5f);
		static char buf1[128] = "";
		ImGui::Text("Input Video Path: ");
		ImGui::SameLine();
		ImGui::InputText("", buf1, 128);
		//ImGui::SetNextWindowSize(ImVec2(640, 720));
		float video_feed_width = ImGui::GetWindowContentRegionWidth() * 0.5f;
		float video_feed_height = ImGui::GetWindowHeight() - 50;
		ImGui::BeginChild("Video Feed", ImVec2(video_feed_width, video_feed_height), false);
		ImGui::Image((void*)snoop_tex, ImVec2(frame.size().width, frame.size().height));
		ImGui::EndChild();
		ImGui::End();
		render_window(window);
	}
	glDeleteTextures(1, &snoop_tex);
	ui_cleanup(window);

	/*cv::VideoCapture video("http://127.0.0.1:8000/campus4-c0.avi");
	cv::Mat frame;
	bool success;

	auto bg_model = cv::createBackgroundSubtractorMOG2();

	while (true)
	{
		success = video.read(frame);
		if (!success)
		{
			std::cout << "Can't read video frame!!";
			break;
		}

		cv::Mat frame_gray = cvt_color(frame, cv::COLOR_BGR2GRAY);
		cv::Mat mask = mog2(frame_gray, bg_model);
		mask = dilate(mask, 3);

		cv::Mat with_contours;
		frame.copyTo(with_contours);

		auto contours = find_contours(mask);

		cv::drawContours(with_contours, contours, -1, cv::Scalar(0, 255, 0));
		cv::imshow("with_contours", with_contours);

		cv::Rect box;
		for (auto& c : contours)
		{
			if (cv::contourArea(c) < 1000 || cv::contourArea(c) > 5000)
				continue;
			box = cv::boundingRect(c);
			cv::rectangle(frame, box, cv::Scalar(0, 0, 255));
		}

		cv::imshow("frame", frame);
		cv::imshow("mask", mask);

		int key = cv::waitKey(1);
		if (key == 'q')
			break;
	}
	cv::destroyAllWindows();*/
	
	return 0;
}