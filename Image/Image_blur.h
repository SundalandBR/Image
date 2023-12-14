#pragma once
#include"Image_opencv.h"
#include "GuidedFilter.h"


void Image_blur(cv::InputArray src, cv::OutputArray dst, double redius, double reg);
