#pragma once

#include <QWidget>
#include "ui_Image_curve2.h"
#include <QtCharts>
#include <qevent.h>
#include "CurveChartView.h"
#include <qlist.h>
#include "Image_opencv.h"
#include "Curve_Spline.h"

class Image_curve2 : public QWidget
{
	Q_OBJECT

public:
	Image_curve2(QWidget* parent = nullptr);
	~Image_curve2();
	QChart* create();
	void updatedata(QPointF point);
	void updateMat();

private:

	Ui::Image_curve2Class ui;
	QChart* chart;
	CurveChartView* chartview;
	QLineSeries* lineseries;
	QScatterSeries* light_scatter;
	QLineSeries* light_line;
	QScatterSeries* saturation_scatter;
	QLineSeries* saturation_line;

	bool in_point;
	bool press_;
	int x_index;
	QList<QPointF>* target_data;
	QList<QPointF> target_data_light;
	QList<QPointF> target_data_saturation;
	QList<QPointF> org_data;
	double lut_light[256];
	double lut_saturation[256];
	double* lut;

private slots:
	void PointHoverd(const QPointF& point, bool state);
	void slotTargetPoint(QPointF point);
	void slotMousePress(bool press);
	void choose_channel(QString channel);
	void on_clicked_enterbutton();
signals:
	void signalsendmat(cv::Mat);
};
