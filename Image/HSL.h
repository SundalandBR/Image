#pragma once

#include <QWidget>
#include "ui_HSL.h"
#include "Image_HSL.h"

class HSL : public QWidget
{
	Q_OBJECT

public:
	HSL(QWidget *parent = nullptr);
	~HSL();

private:
	Ui::HSLClass ui;

private:
	cv::Mat src;
	cv::Mat _dst;
	one_pixel_hsl** hsl_src;
	one_pixel_hsl** hsl_dst;

private slots:

	void enter();
	void exit();
	void h_slider_changed();
	void h_slider2_changed();
	void h_slider3_changed();
	void h_slider4_changed();
	void h_slider5_changed();
	void h_slider6_changed();

	void s_slider_changed();
	void s_slider2_changed();
	void s_slider3_changed();
	void s_slider4_changed();
	void s_slider5_changed();
	void s_slider6_changed();

	void l_slider_changed();
	void l_slider2_changed();
	void l_slider3_changed();
	void l_slider4_changed();
	void l_slider5_changed();
	void l_slider6_changed();

signals:
	void send_mat(cv::Mat);

};
