#pragma once

#include <QWidget>
#include "ui_saturation.h"
#include "Image_HSL.h"

class saturation : public QWidget
{
	Q_OBJECT

public:
	saturation(QWidget *parent = nullptr);
	~saturation();

private:
	Ui::saturationClass ui;

private:
	cv::Mat src;

private slots:
	void exit();
	void saturation_slider_changed();

signals:
	void send_mat(cv::Mat);

};
