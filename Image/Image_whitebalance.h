#pragma once
#include "Image_opencv.h"
#include <vector>

using namespace std;


void whilepatch(cv::InputArray src, cv::OutputArray dst, int percent);
double percentileValue(cv::Mat src, int percent);