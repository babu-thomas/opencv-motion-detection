#include "utils.h"

cv::Mat resize(const cv::Mat& image, int width, int height, cv::InterpolationFlags type)
{
	cv::Size new_size;
	cv::Size cur_size = image.size();

	if (width == 0)
	{
		if (height == 0)
			return image;
		else
		{
			float ratio = height / (float)cur_size.height;
			new_size.width = (int)(cur_size.width  * ratio);
			new_size.height = height;
		}
	}
	else
	{
		if (height == 0)
		{
			float ratio = width / (float)cur_size.width;
			new_size.width = width;
			new_size.height = (int)(cur_size.height * ratio);
		}
		else
		{
			new_size.width = width;
			new_size.height = height;
		}
	}

	cv::Mat resized;
	cv::resize(image, resized, new_size, cv::INTER_AREA);

	return resized;
}

cv::Mat cvt_color(const cv::Mat& image, cv::ColorConversionCodes type)
{
	cv::Mat result;

	cv::cvtColor(image, result, type);

	return result;
}

std::vector<std::vector<cv::Point>> find_contours(const cv::Mat& image,
	cv::RetrievalModes retrieval_mode, cv::ContourApproximationModes approx_mode)
{
	cv::Mat img_copy;
	std::vector<std::vector<cv::Point>> contours;
	image.copyTo(img_copy);
	cv::findContours(img_copy, contours, retrieval_mode, approx_mode);
	
	return contours;
}
