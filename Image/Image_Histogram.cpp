// 直方图均衡化

#include "Image_Histogram.h"
 


#define PIEXL_RANGE(value, min, max)  ( (value) > (max) ? (max) : (((value) < (min)) ? (min) : (value)) )




void cdf(InputArray src, OutputArray dst,struct _cdf_ *_CDF) {

	Mat _src = src.getMat();
	Mat_<uchar>::iterator _src_begin = _src.begin<uchar>();
	Mat_<uchar>::iterator _src_end = _src.end<uchar>();
	int grey_pixel[256];
	memset(_CDF->array, 0, sizeof(_CDF->array));
	memset(grey_pixel, 0, sizeof(grey_pixel));
	_CDF->MAX_CDF = INT_MIN;
	_CDF->MIN_CDF = INT_MAX;
	int i = 0;
	for (; _src_begin != _src_end; _src_begin++) {
		i++;
		int t = (*_src_begin);
		grey_pixel[t]++;
	}
	_CDF->array[0] = grey_pixel[0];
	for (int i = 1; i < 256; i++) {
		_CDF->array[i] = _CDF->array[i - 1] + grey_pixel[i];
	}
	for (int i = 0; i < 256; i++) {
		_CDF->array[i] = grey_pixel[i] == 0 ? 1 : _CDF->array[i];
		_CDF->MAX_CDF = _CDF->MAX_CDF > _CDF->array[i] ? _CDF->MAX_CDF : _CDF->array[i];
		_CDF->MIN_CDF = _CDF->MIN_CDF < _CDF->array[i] ? _CDF->MIN_CDF : _CDF->array[i];
	}

	cv::Mat lookuptable(1, 256, CV_8U);
	printf("MAX %d  MIN %d \n", _CDF->MAX_CDF, _CDF->MIN_CDF);
	for (int i = 0; i < 256; i++) {
		lookuptable.at<uchar>(i) = round( PIEXL_RANGE((255 * (_CDF->array[i] - _CDF->MIN_CDF) / (_CDF->MAX_CDF - _CDF->MIN_CDF)), 0, 255));
		printf("%d %d %d\n", i, lookuptable.at<uchar>(i), _CDF->array[i]);
	}
	cv::LUT(src, lookuptable, dst);
	printf("\n\n\n\n");
	return;
}


void Imgae_Histogram(InputArray src,OutputArray dst) {


	Mat _src = src.getMat();
	dst.create(_src.size(), _src.type());
	Mat _dst = dst.getMat();
	std::vector<cv::Mat> p, q;
	cv::split(_src, p);
	for (int i = 0; i < _src.channels(); i++) {

		Mat t;
		struct _cdf_* _CDF;
		_CDF = (struct _cdf_*)malloc(sizeof(struct _cdf_));
		cdf(p[i],p[i], _CDF);
		q.push_back(p[i]);
		free(_CDF);

	}
	cv::merge(q, _dst);
	return;
}