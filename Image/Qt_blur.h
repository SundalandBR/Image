#pragma once

#include <QWidget>
#include "ui_Qt_blur.h"
#include "Image_blur.h"
class Qt_blur : public QWidget
{
	Q_OBJECT

public:
	Qt_blur(QWidget *parent = nullptr);
	~Qt_blur();

private:
	Ui::Qt_blurClass ui;

private:
	cv::Mat src;

private slots:
	void exit();
	void blur_slider_changed();

signals:
	void send_mat(cv::Mat);

};
