#include "Image_blur.h"

//平滑

void Image_blur(cv::InputArray src, cv::OutputArray dst, double redius, double reg) {
	/*
	* edius 导向滤波半径
	* reg 导向滤波正则化参数
	*/

	cv::Mat _gray_src;
	cv::cvtColor(src, _gray_src, cv::COLOR_BGR2GRAY);
	cv::Mat _src = src.getMat();
	dst.create(_src.size(), _src.type());
	cv::Mat _dst = dst.getMat();
	std::vector<cv::Mat> p, q;
	split(_src, p);
	for (int i = 0; i < _src.channels(); i++) {
		p[i] = GuideFilter(_gray_src, p[i], redius, reg);
		q.push_back(p[i]);
	}
	merge(q, _dst);
}


