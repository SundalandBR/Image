#pragma once
#include "Image_opencv.h"

cv::Mat GuideFilter(cv::Mat I, cv::Mat p, int redius, double reg);