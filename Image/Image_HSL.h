#pragma once
#include "Image_opencv.h"



typedef enum base_color {
	RED,
	YELLOW,
	GREEN,
	BULEGREEN,
	BULE,
	MEGENTA,
}basecolor;

typedef struct one_pixel_hsl
{
	float h;
	float s;
	float l;

}hsl;


	void HSL2RGB(cv::InputArray src, cv::OutputArray dst, hsl** hsl);
	one_pixel_hsl** RGB2HSL(cv::InputArray src);
	float hue2rgb(float q, float p, float t);
	void HUE_(hsl** hsl, int offset, int rows, int cols);
	void channel_hsl_h(hsl** hsl_src, hsl** hsl_dst, int offset, basecolor base_color_n, int rows, int cols);
	void channel_hsl_l(hsl** hsl_src, hsl** hsl_dst, int offset, basecolor base_color_n, int rows, int cols);
	void channel_hsl_s(hsl** hsl_src, hsl** hsl_dst, int offset, basecolor base_color_n, int rows, int cols);
	void Image_HSL(cv::InputArray src, cv::OutputArray dst, int offset, basecolor channel, int HSL_OBJECT, hsl** hsl_src, hsl** hsl_dst);
	void Image_HUE(cv::InputArray src, cv::OutputArray dst, int offset);
	void Image_Light(cv::InputArray src, cv::OutputArray dst, int offset);
	void Image_Saturation(cv::InputArray src, cv::OutputArray dst, int offset);
