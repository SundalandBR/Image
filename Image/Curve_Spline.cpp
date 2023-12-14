#include "Curve_Spline.h"

spline_point spline(vector<pair<int, int>> s) {

	vector<pair<int, int>>point;
	for (int i = 0; i < s.size(); i++) {
		point.emplace_back(s[i].first, s[i].second);
	}
	int n = point.size() - 1; // 线段数 == 方程数 == 插值点数-1
	coefficient* coe = new coefficient[n];
	double* h, * d;
	double* a, * b, * c, * f, * m;
	double temp;

	h = new double[6 * n + 4];
	if (h == NULL)
	{
		exit(1);
	}
	/*n是方程数
	根据计算可以得到n个方程
	但是有n+2个变量
	所以还需要引入两个方程
	*/
	d = h + n;                                            /* h	n	   0,1,...,(n-1)	*/
	a = d + n;                                            /* d	n	   0,1,...,(n-1)	*/
	b = a + (n + 1);                                      /* a  n+1    0,1,...,(n)		*/
	c = b + (n + 1);                                      /* b  n+1    0,1,...,(n)		*/
	f = c + (n + 1);                                      /* c  n+1    0,1,...,(n)		*/
	m = b;												  /* f  n+1    0,1,...,(n)		*/

	/*
	* 计算 步长h 与 方程等式右边系数d
	* di = (yi+1 - yi)/hi
	*/
	for (int i = 0; i < n; i++) {
		h[i] = point[i + 1].first - point[i].first;
		d[i] = (point[i + 1].second - point[i].second) / h[i];
	}

	//自然边界条件时
	f[0] = 0;
	f[n] = 0;
	a[0] = 0;
	c[n] = 0;
	c[0] = 0;
	a[n] = 0;
	b[0] = 1;
	b[n] = 1;

	//初始化矩阵系数
	for (int i = 1; i < n; i++)
	{
		a[i] = h[i - 1];
		b[i] = 2 * (h[i - 1] + h[i]);
		c[i] = h[i];
		f[i] = 6 * (d[i] - d[i - 1]);
	}


	c[0] /= b[0];
	f[0] /= b[0];
	for (int i = 1; i < n; i++) {
		temp = 1.f / (b[i] - a[i] * c[i - 1]);
		c[i] *= temp;
		f[i] = (f[i] - a[n] * f[n - 1]) * temp;
	}
	m[n] = f[n];
	for (int i = n - 1; i >= 0; i--) {
		m[i] = f[i] - c[i] * m[i + 1];
	}

	for (int i = 0; i < n; i++) {
		coe[i].a3 = (m[i + 1] - m[i]) / (6 * h[i]);
		coe[i].b2 = m[i] / 2;
		coe[i].c1 = d[i] - (h[i] / 6) * (2 * m[i] + m[i + 1]);
		coe[i].d0 = point[i].second;
	}
	delete h;

	spline_point spoint;
	spoint.cnt = n + 1;
	spoint.coe = coe;
	spoint.point = point;

	return spoint;
}

/*
* 输入横坐标x,spline曲线系数;
* 返回纵坐标y;
*/
double spline_Coordinate(int x, _spline_point spline_point) {
	int num = 0;
	int xi;
	for (num; num < spline_point.cnt - 1; num++) {
		if (x <= spline_point.point[num + 1].first && x >= spline_point.point[num].first) {
			xi = spline_point.point[num].first;
			break;
		}
	}
	if (num >= spline_point.cnt) {
		return -1;
	}
	x = x - xi;
	coefficient coe = spline_point.coe[num];
	double t = coe.a3 * x * pow(x) + coe.b2 * pow(x) + coe.c1 * x + coe.d0;
	return ((t < 0 ? 0 : t) > 255 ? 255 : (t < 0 ? 0 : t));
}