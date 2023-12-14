#pragma once
#include <utility>
#include <vector>

#define pow(x) (x*x)
#define abs(x) (x > 0 ? x : -x)
#define swap(x,y,t) (t) = (y) ; (y) = (x) ; (x) = (t);

using namespace std;

typedef struct _coefficient
{
	double a3;
	double b2;
	double c1;
	double d0;
}coefficient;


typedef struct _spline_point
{
	int cnt;						 //插值点个数
	vector<pair<int, int>>point;	 //插值点坐标
	coefficient* coe;				 //方程系数
}spline_point;

spline_point spline(vector<pair<int, int>> s);
double spline_Coordinate(int x, _spline_point spline_point);