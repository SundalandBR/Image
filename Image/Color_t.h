#pragma once

#include <QWidget>
#include "ui_Color_t.h"
#include "Image_opencv.h"
#include "Image_ColorTemperature.h"

class Color_t : public QWidget
{
	Q_OBJECT

public:
	Color_t(QWidget *parent = nullptr);
	~Color_t();

private:
	cv::Mat src;

private slots:
	void exit();
	void color_t_slider_changed();

signals:
	void send_mat(cv::Mat);

private:
	Ui::Color_tClass ui;
};
