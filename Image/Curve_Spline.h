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
	int cnt;						 //��ֵ�����
	vector<pair<int, int>>point;	 //��ֵ������
	coefficient* coe;				 //����ϵ��
}spline_point;

spline_point spline(vector<pair<int, int>> s);
double spline_Coordinate(int x, _spline_point spline_point);