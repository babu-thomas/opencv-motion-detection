#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "utils.h"
#include "filters.h"

int main(int argc, char *argv[])
{
	cv::VideoCapture video("http://127.0.0.1:8000/campus4-c0.avi");
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
		frame = cvt_color(frame, cv::COLOR_BGR2GRAY);
		cv::Mat mask = mog2(frame, bg_model);

		cv::imshow("frame", frame);
		cv::imshow("mask", mask);

		int key = cv::waitKey(1);
		if (key == 'q')
			break;
	}
	cv::destroyAllWindows();
	return 0;
}