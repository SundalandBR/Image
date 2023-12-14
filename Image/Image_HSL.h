#pragma once
#include "Image_opencv.h"
using namespace cv;


#define RED 0
#define YELLOW 60
#define GREEN 120
#define BULEGREEN 180
#define BULE 240
#define MEGENTA 300

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
void channel_hsl(hsl** hsl, int offset, int base_color, int rows, int cols);
