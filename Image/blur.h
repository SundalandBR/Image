#pragma once

#include <QWidget>
#include "ui_blur.h"
#include "Image_blur.h"
class blur : public QWidget
{
	Q_OBJECT

public:
	blur(QWidget *parent = nullptr);
	~blur();

private:
	cv::Mat src;

private slots:
	void exit();
	void blur_slider_changed();

signals:
	void send_mat(cv::Mat);

private:
	Ui::blurClass ui;
};
