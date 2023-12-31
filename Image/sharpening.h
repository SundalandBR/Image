#pragma once

#include <QWidget>
#include "ui_sharpening.h"
#include "Image_sharpening.h"

class sharpening : public QWidget
{
	Q_OBJECT

public:
	sharpening(QWidget *parent = nullptr);
	~sharpening();

private:
	Ui::sharpeningClass ui;

private:
	cv::Mat src;
	int sharpening_kernel_num;

private slots:
	void exit();
	void sharp_combox_slider_changed();
	void sharp_combox_value(QString s);

signals:
	void send_mat(cv::Mat);

};
