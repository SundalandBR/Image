#pragma once
#include "Image_opencv.h"
#include <algorithm>
#include <iostream>

void Image_exposure(cv::InputArray _src, cv::OutputArray _dst, double strength);