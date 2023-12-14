#pragma once
#include "Image_opencv.h"


using namespace cv;
static struct _cdf_
{
	int array[256];
	int MAX_CDF;
	int	MIN_CDF;
};


void Imgae_Histogram(cv::InputArray src, cv::OutputArray dst);
static void cdf(cv::InputArray src, struct _cdf_* _CDF);