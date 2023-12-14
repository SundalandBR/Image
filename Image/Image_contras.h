#pragma once
#include "Image_opencv.h"

void Image_contras(cv::InputArray Input, cv::OutputArray Output, double beta,double alpha); // 对比度和亮度
void Image_contras_gamma(cv::InputArray _src, cv::OutputArray _dst, double gamma);
void Image_contras_bright(cv::InputArray Input, cv::OutputArray Output, double beta, double alpha);