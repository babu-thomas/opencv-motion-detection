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

		ImGui::SetNextWindowSize(ImVec2(640, 720));
		ImGui::Begin("Video Feed");
		ImGui::Text("Hello");
		ImGui::End();
		render_window(window);
	}
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