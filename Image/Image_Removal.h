#pragma once

#include "GuidedFilter.h"
#include "Image_opencv.h"


cv::Mat GetDarkChannel(const cv::Mat M, const int radius, double** MinChannel);
double** GetMinChannel(cv::Mat M);
double* Get_atmospheric_light(cv::Mat Input, cv::Mat DarkChannel);
cv::Mat Get_transmission(cv::Mat Input, cv::Mat DarkChannel, double omege, double* atmospheric_light, int radius);
cv::Mat Haze_Removal(cv::Mat Input, double* a, cv::Mat t, double t0 );
cv::Mat Image_Removal_GuidedFilter_gray(cv::Mat Input, int radius, double reg );
cv::Mat Imgae_Removal(cv::Mat Input, int radius = 7, double omege = 0.95, bool GuideFilterFORt = false, int Guidefilter_radius = 75, double reg = 0.1 * 0.1,double t0 = 0.2);