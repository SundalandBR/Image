#pragma once

#include "Image_opencv.h"

using namespace cv;
void Image_sharpening(InputArray src, OutputArray dst, int filter, int reg);