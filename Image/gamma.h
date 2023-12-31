#pragma once

#include <QWidget>
#include "ui_gamma.h"
#include "Image_contras.h"

class gamma : public QWidget
{
	Q_OBJECT

public:
	gamma(QWidget *parent = nullptr);
	~gamma();

private:
	Ui::gammaClass ui;

private:
	cv::Mat src;

private slots:
	void exit();
	void gamma_slider_changed();

signals:
	void send_mat(cv::Mat);

};
