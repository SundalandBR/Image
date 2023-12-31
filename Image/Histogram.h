#pragma once

#include <QWidget>
#include "ui_Histogram.h"
#include "ViewLabel.h"
#include "Image_Histogram.h"
#include "qtimer.h"


class Histogram : public QWidget
{
	Q_OBJECT

public:
	Histogram(QWidget *parent = nullptr);
	~Histogram();

private:
	Ui::HistogramClass ui;
	cv::Mat dst;
	int label_width_;
	int label_height_;
	void print_image(ViewLabel* tar, cv::Mat image);
private slots:
	void enter();
	void his_fun();


signals:
	void send_mat(cv::Mat);

};
