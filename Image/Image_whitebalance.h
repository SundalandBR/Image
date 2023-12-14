#pragma once
#include "Image_opencv.h"
#include <vector>
using namespace cv;
using namespace std;


void whilepatch(InputArray src, OutputArray dst, int percent);
double percentileValue(cv::Mat src, int percent);