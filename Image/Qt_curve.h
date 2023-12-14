#pragma once

#include <QWidget>
#include "ui_Qt_curve.h"
#include <QtCharts>
#include <qevent.h>
#include "CurveChartView.h"
#include <qlist.h>
#include "Image_opencv.h"

class Qt_curve : public QWidget
{
	Q_OBJECT

public:
	Qt_curve(QWidget *parent = nullptr);
	~Qt_curve();
	QChart* create() ;
	void updatedata(QPointF point);
	void updateMat();

private:
	Ui::Qt_curveClass ui;
	QChart* chart;
	CurveChartView* chartview;
	QScatterSeries* series_point;
	QLineSeries* lineseries;
	QScatterSeries* rgb_scatter;
	QLineSeries* rgb_line;
	QScatterSeries* red_scatter;
	QLineSeries* red_line;
	QScatterSeries* blue_scatter;
	QLineSeries* blue_line;
	QScatterSeries* green_scatter;
	QLineSeries* green_line;
	bool in_point;
	bool press_;
	int x_index;
	QList<QPointF> *target_data;
	QList<QPointF> target_data_rgb;
	QList<QPointF> target_data_r;
	QList<QPointF> target_data_b;
	QList<QPointF> target_data_g;
	QList<QPointF> org_data;
	double lut_rgb[256];
	double lut_r[256];
	double lut_g[256];
	double lut_b[256];
	double *lut;
private slots:
	void PointHoverd(const QPointF& point, bool state);
	void slotTargetPoint(QPointF point);
	void slotMousePress(bool press);
	void choose_channel(QString channel);
	void on_clicked_enterbutton();
signals:
	void signalsendmat(cv::Mat);


};
