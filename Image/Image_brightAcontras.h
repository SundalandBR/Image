#pragma once

#include <QMainWindow>
#include "ui_Image_brightAcontras.h"
#include "Image_opencv.h"
#include "Image_contras.h"
class Image_brightAcontras : public QMainWindow
{
	Q_OBJECT

public:
	Image_brightAcontras(QWidget *parent = nullptr);
	~Image_brightAcontras();

private:
	Ui::Image_brightAcontrasClass ui;
	cv::Mat src;

private slots:
	//void s_bright(double beta);
	//void s_contras_gamma(double alpha);
	//void s_contras(double alpha);
	void s_contras(double beta,double alpha);
	void s_send();
	void send_mat();


signals:
	void send_alpha(double alpha);
	void send_strength(double strength);
	void send_beta(double beta);
	void send_val(double beta, double alpha);
	void bright_and_constras_mat(cv::Mat output);

};
