#include "Image_exposure.h"

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))


void Image_exposure(cv::InputArray _src, cv::OutputArray _dst, double strength) {
	cv::Mat src = _src.getMat();
	_dst.create(src.size(), src.type());
	cv::Mat dst = _dst.getMat();

	uchar* pInputImg = src.data;
	uchar* pOutputImg = dst.data;
	int step = src.step;

	for (int i = 0; i < src.rows * step; i++) {
		pOutputImg[i] = (uchar)min(255, max(0, pInputImg[i] * pow(2, strength)));
	}

}