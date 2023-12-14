#include "Image_sharpening.h"

void Image_sharpening(InputArray src, OutputArray dst, int filter, int reg) {
	/*
	* reg ÂË²¨Ç¿¶È [0,100]
	*/
	Mat kernel;

	switch (filter)
	{

	case 0:
		kernel = (Mat_<int>(3, 3) <<
			0, -1, 0,
			-1, 4, -1,
			0, -1, 0
			);
		break;
	case 1:
		kernel = (Mat_<int>(3, 3) <<
			-1, -1, -1,
			-1, 8, -1,
			-1, -1, -1
			);
		break;
	case 2:
		kernel = (Mat_<int>(3, 3) <<
			0, -1, 0,
			-1, 5, -1,
			0, -1, 0
			);
		break;
	case 3:
		kernel = (Mat_<int>(3, 3) <<
			1, -2, 1,
			-2, 5, -2,
			1, -2, 1
			);
	default:
		kernel = (Mat_<int>(3, 3) <<
			0, -1, 0,
			-1, 4, -1,
			0, -1, 0
			);
		break;
	}

	Mat _src = src.getMat();
	dst.create(_src.size(), _src.type());
	Mat _dst = dst.getMat();
	Mat t;
	_src.copyTo(t);
	filter2D(t, t, t.depth(), kernel);
	_dst = _src + t * 0.01 * reg;
}