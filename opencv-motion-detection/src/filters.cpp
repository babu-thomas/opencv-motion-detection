#include "filters.h"
#include "utils.h"


cv::Mat gaussian_blur(const cv::Mat& image)
{
	cv::Mat blur;
	cv::Size kernel_size(21, 21);

	cv::GaussianBlur(image, blur, kernel_size, 0);

	return blur;
}

cv::Mat frame_diff(const cv::Mat& prev, const cv::Mat& cur, const cv::Mat& next)
{
	cv::Mat diff1, diff2, result;

	cv::Mat prev_gray = cvt_color(prev, cv::COLOR_BGR2GRAY);
	cv::Mat cur_gray = cvt_color(cur, cv::COLOR_BGR2GRAY);
	cv::Mat next_gray = cvt_color(next, cv::COLOR_BGR2GRAY);
	cv::absdiff(prev_gray, next_gray, diff1);
	cv::imshow("diff1", diff1);
	cv::absdiff(cur_gray, next_gray, diff2);
	cv::imshow("diff2", diff2);
	cv::bitwise_and(diff1, diff2, result);
	cv::imshow("result", result);
	cv::threshold(result, result, 35, 255, cv::THRESH_BINARY);
	
	return result;
}

cv::Mat dilate(const cv::Mat& img, int iterations)
{
	cv::Mat result;

	cv::dilate(img, result, cv::Mat(), cv::Point(-1, -1), iterations);

	return result;
}

cv::Mat mog2(const cv::Mat& frame, cv::Ptr<cv::BackgroundSubtractor> mog2)
{
	cv::Mat fg_mask;

	mog2->apply(frame, fg_mask);
	cv::threshold(fg_mask, fg_mask, 128, 255, cv::THRESH_BINARY);

	return fg_mask;
}
