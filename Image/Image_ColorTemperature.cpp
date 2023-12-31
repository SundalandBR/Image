
#include "Image_ColorTemperature.h"

void Image_ColorTemperature(cv::InputArray src, cv::OutputArray dst, int n) {
	int level = n / 2;
	cv::Mat _src = src.getMat();
	dst.create(src.size(), src.type());
	cv::Mat _dst = dst.getMat();
	for (int i = 0; i < _src.rows; i++) {
		for (int j = 0; j < _src.cols; j++) {
			
			//BGR
			uchar B = _src.at<cv::Vec3b>(i, j)[0];
			uchar G = _src.at<cv::Vec3b>(i, j)[1];
			uchar R = _src.at<cv::Vec3b>(i, j)[2];

			_dst.at<cv::Vec3b>(i, j)[0] = cv::saturate_cast<uchar>(B - level);
			_dst.at<cv::Vec3b>(i, j)[1] = cv::saturate_cast<uchar>(G + level);
			_dst.at<cv::Vec3b>(i, j)[2] = cv::saturate_cast<uchar>(R + level);

		}
	}
}