#pragma once

#include <QWidget>
#include "ui_exposure.h"
#include "Image_exposure.h"

class exposure : public QWidget
{
	Q_OBJECT

public:
	exposure(QWidget *parent = nullptr);
	~exposure();

private:
	Ui::exposureClass ui;
	cv::Mat src;

private slots:
	void exit();
	void exposure_slider_changed();


signals:
	void send_mat(cv::Mat);

};
