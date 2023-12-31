#pragma once

#include <QWidget>
#include "ui_HUE.h"
#include "Image_HSL.h"

class HUE : public QWidget
{
	Q_OBJECT

public:
	HUE(QWidget *parent = nullptr);
	~HUE();

private:
	Ui::HUEClass ui;

private:
	cv::Mat src;

private slots:
	void exit();
	void hue_slider_changed();

signals:
	void send_mat(cv::Mat);

};
