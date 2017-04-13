#pragma once

#include <opencv2/opencv.hpp>


cv::Mat gaussian_blur(const cv::Mat& image);

/**
 * Motion detection using frame differencing. Takes 3 consecutive frames. Returns foreground mask.
 **/
cv::Mat frame_diff(const cv::Mat& prev, const cv::Mat& cur, const cv::Mat& next);

cv::Mat dilate(const cv::Mat& img, int iterations = 1);

/**
 * Motion detection using Mixture of Gaussians for background foreground segmentation.
 * Returns foreground mask.
 **/
cv::Mat mog2(const cv::Mat& frame, cv::Ptr<cv::BackgroundSubtractor> mog2);