#include "Image_HSL.h"
#define vmax(r,g,b)  ((r > g ? r : g) > b ?  (r > g ? r : g) : b)
#define vmin(r,g,b)  ((r < g ? r : g) < b ?  (r < g ? r : g) : b)

struct one_pixel_hsl** RGB2HSL(cv::InputArray src) {
	cv::Mat _src;
	src.getMat().copyTo(_src);
	struct one_pixel_hsl** hsl = new struct one_pixel_hsl*[src.rows()];
	for (int i = 0; i < src.rows(); i++) {
		hsl[i] = new struct one_pixel_hsl[src.cols()];
	}
	for (int i = 0; i < src.rows(); i++) {
		
		for (int j = 0; j < src.cols(); j++) {

			uchar bb = _src.at<cv::Vec3b>(i, j)[0];
			uchar gg = _src.at<cv::Vec3b>(i, j)[1];
			uchar rr = _src.at<cv::Vec3b>(i, j)[2];
			float b = _src.at<cv::Vec3b>(i, j)[0] / 255.f;
			float g = _src.at<cv::Vec3b>(i, j)[1] / 255.f;
			float r = _src.at<cv::Vec3b>(i, j)[2] / 255.f;
			float h;
			float s;
			float l;
			float vmax = vmax(r, g, b);
			float vmin = vmin(r, g, b);
			l = (vmax + vmin) / 2.f;
			if (vmax == vmin) {
				hsl[i][j].h = 0.f;
				hsl[i][j].l = l;
				hsl[i][j].s = 0.f;
				continue;
			}
			float d = vmax - vmin;
			s = l > 0.5 ? d / (2 - vmax - vmin) : d / (vmax + vmin);
			if (vmax == r) h = (g - b) / d + (g < b ? 6 : 0);
			if (vmax == g) h = (b - r) / d + 2;
			if (vmax == b) h = (r - g) / d + 4;
			h *= 60;
			hsl[i][j].h = h;
			hsl[i][j].l = l;
			hsl[i][j].s = s;
		}
	}
	return hsl;
}

void HSL2RGB(cv::InputArray src, cv::OutputArray dst, hsl** hsl) {
	dst.create(src.size(), src.type());
	cv::Mat _dst = dst.getMat();
	for (int i = 0; i < src.rows(); i++) {
		for (int j = 0; j < src.cols(); j++) {
			float r, g, b;
			float h = hsl[i][j].h/360;
			float s = hsl[i][j].s;
			float l = hsl[i][j].l;
			if (s == 0) {
				r = g = b = l;
			}
			else {
				float q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
				float p = 2 * l - q;
				r = hue2rgb(p, q, h + 1.f / 3.f);
				g = hue2rgb(p, q, h);
				b = hue2rgb(p, q, h - 1.f / 3.f);
			}
			_dst.at<cv::Vec3b>(i, j)[0] = b * 255;
			_dst.at<cv::Vec3b>(i, j)[1] = g * 255;
			_dst.at<cv::Vec3b>(i, j)[2] = r * 255;
		}
	}
}

float hue2rgb(float p, float q, float t) {
	if (t < 0) t += 1;
	if (t > 1) t -= 1;
	if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
	if (t < 1.f / 2.f) return q;
	if (t < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t) * 6.f;
	return p;
}
//请务必分清1和l


/*
* 360°/0°红、60°黄、120°绿、180°青、240°蓝、300°洋红
* [-30,30)红色通道 [30,90)黄色通道 [90,120)绿色通道 [150,210)青 [210,270)蓝 [270,330)洋红
* 以颜色像素偏差1为代价，使用查表方式以获取更快的速度
*/

void channel_hsl_h(hsl **hsl_src,hsl** hsl_dst,int offset,basecolor base_color_n,int rows,int cols) {
	int base_color = base_color_n * 60;
	int offset_ = round((offset / 100.f) * 30.f); 
	int left = base_color - 30;
	int right = (base_color + 30 > 360) ? base_color - 330 : (base_color + 30);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int n = round(hsl_src[i][j].h);
			n = n > 330 ? n - 360 : n;
			if (n >= left && n < right) {
				hsl_dst[i][j].h = hsl_src[i][j].h + offset_;
			}
		}
	}
}

void channel_hsl_s(hsl** hsl_src, hsl** hsl_dst, int offset, basecolor base_color_n, int rows, int cols) {
	int base_color = base_color_n * 60;
	int hsl_table[360];
	int left = base_color - 30;
	int right = (base_color + 30 > 360) ? base_color - 330 : (base_color + 30);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int n = round(hsl_src[i][j].h);
			n = n > 330 ? n - 360 : n;
			if (n >= left && n < right) {
				double t = (hsl_src[i][j].s + offset / 100.f);
				hsl_dst[i][j].s = (t > 1 ? 1 : t) < 0 ? 0 : (t > 1 ? 1 : t);
			}
		}
	}

}

void channel_hsl_l(hsl** hsl_src, hsl** hsl_dst, int offset, basecolor base_color_n, int rows, int cols) {
	int base_color = base_color_n * 60;
	int hsl_table[360];
	int left = base_color - 30;
	int right = (base_color + 30 > 360) ? base_color - 330 : (base_color + 30);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int n = round(hsl_src[i][j].h);
			n = n > 330 ? n - 360 : n;
			if (n >= left && n < right) {
				double t = (hsl_src[i][j].l + offset / 100.f);
				hsl_dst[i][j].l = (t > 1 ? 1 : t) < 0 ? 0 : (t > 1 ? 1 : t);
			}
		}
	}
}

void HUE_(hsl** hsl,int offset, int rows, int cols) {
	int n = round(25 * offset / 100.f);
	int hsl_table[360][2];
	for (int i = 0; i < 360; i++) {
		//色相
		hsl_table[i][0] = (i + n)%360;
		//亮度
		//hsl_table[i][1] = i;
		hsl_table[i][1] = cv::saturate_cast<uchar>(i + (4 * n / 3));
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int h = hsl[i][j].h;
			int l = hsl[i][j].l * 255;
			hsl[i][j].h = hsl_table[h][0];
			hsl[i][j].l = hsl_table[l][1] / 255.f;
		}
	}
}


void Image_HSL(cv::InputArray src, cv::OutputArray dst,int offset, basecolor channel,int HSL_OBJECT,hsl**hsl_src,hsl**hsl_dst) {
	cv::Mat _src = src.getMat();
	dst.create(src.size(), src.type());
	cv::Mat _dst = dst.getMat();
	if (HSL_OBJECT == 1) {
		channel_hsl_h(hsl_src, hsl_dst, offset, channel, _src.rows, _src.cols);
	}
	else if(HSL_OBJECT == 2){
		channel_hsl_l(hsl_src, hsl_dst, offset, channel, _src.rows, _src.cols);
	}
	else if(HSL_OBJECT ==3){
		channel_hsl_s(hsl_src, hsl_dst, offset, channel, _src.rows, _src.cols);
	}
	HSL2RGB(_src, _dst, hsl_dst);
}

/*
* HUE 色调
* offset 色调调节量 [-100,100]
* 色相与亮度以4/3的比例增长
* 色相范围[-30,30] 亮度范围[0,255]
*/
void Image_HUE(cv::InputArray src, cv::OutputArray dst, int offset) {
	cv::Mat _src = src.getMat();
	dst.create(src.size(), src.type());
	cv::Mat _dst = dst.getMat();
	one_pixel_hsl** hsl;
	hsl = RGB2HSL(src);
	HUE_(hsl, offset,_src.rows, _src.cols);
	HSL2RGB(_src, _dst, hsl);
}


void Image_Saturation(cv::InputArray src, cv::OutputArray dst, int offset) {
	cv::Mat _src = src.getMat();
	dst.create(src.size(), src.type());
	cv::Mat _dst = dst.getMat();
	one_pixel_hsl** hsl;
	hsl = RGB2HSL(src);
	int hsl_table[256];
	for (int i = 0; i < 256; i++) {
		//饱和度
		hsl_table[i] = (i + offset*(255-i)/100.f );
	}
	hsl_table[0] = 0;
	for (int i = 0; i < src.rows(); i++) {
		for (int j = 0; j < src.cols(); j++) {
			int s = hsl[i][j].s * 255;
			hsl[i][j].s = hsl_table[s] / 255.f;
		}
	}
	HSL2RGB(_src, _dst, hsl);
}

void Image_Light(cv::InputArray src, cv::OutputArray dst, int offset) {
	cv::Mat _src = src.getMat();
	dst.create(src.size(), src.type());
	cv::Mat _dst = dst.getMat();
	one_pixel_hsl** hsl;
	hsl = RGB2HSL(src);
	int hsl_table[256];
	for (int i = 0; i < 256; i++) {
		hsl_table[i] = cv::saturate_cast<uchar>(i + offset);
	}
	for (int i = 0; i < src.rows(); i++) {
		for (int j = 0; j < src.cols(); j++) {
			int l = hsl[i][j].l * 255;
			hsl[i][j].l = hsl_table[l] / 255.f;
		}
	}
	HSL2RGB(_src, _dst, hsl);
}