#pragma once

#include <vector>
#include <opencv2/opencv.hpp>


/**
* Resizes images, maintaining aspect ratio if only one of width or height is specified.
**/
cv::Mat resize(const cv::Mat& image, int width = 0, int height = 0,
	cv::InterpolationFlags type = cv::INTER_AREA);

cv::Mat cvt_color(const cv::Mat& image, cv::ColorConversionCodes type);

std::vector<std::vector<cv::Point>> find_contours(const cv::Mat& image,
	cv::RetrievalModes retrieval_mode = cv::RETR_EXTERNAL,
	cv::ContourApproximationModes approx_mode = cv::CHAIN_APPROX_SIMPLE);