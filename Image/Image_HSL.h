#pragma once
#include "Image_opencv.h"
using namespace cv;


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


void HSL2RGB(InputArray src, OutputArray dst, hsl** hsl);
one_pixel_hsl** RGB2HSL(InputArray src);
float hue2rgb(float q, float p, float t);
void HUE(hsl** hsl,int offset, int rows, int cols);
void channel_hsl(hsl** hsl, int offset, basecolor base_color_n, int rows, int cols);
void Image_HSL(InputArray src, OutputArray dst, int offset, basecolor channel);
void Image_HUE(InputArray src, OutputArray dst, int offset);