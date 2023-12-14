
#include "Image_ColorTemperature.h"

void Image_ColorTemperature(InputArray src, OutputArray dst, int n) {
	int level = n / 2;
	Mat _src = src.getMat();
	dst.create(src.size(), src.type());
	Mat _dst = dst.getMat();
	for (int i = 0; i < _src.rows; i++) {
		for (int j = 0; j < _src.cols; i++) {
			
			//BGR
			uchar B = _src.at<Vec3b>(i, j)[0];
			uchar G = _src.at<Vec3b>(i, j)[1];
			uchar R = _src.at<Vec3b>(i, j)[2];

			_dst.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(B - level);
			_dst.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(G + level);
			_dst.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(R + level);

		}
	}
}