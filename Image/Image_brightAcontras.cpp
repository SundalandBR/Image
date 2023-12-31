#include "Image_brightAcontras.h"

extern cv::Mat Input;


Image_brightAcontras::Image_brightAcontras(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	Input.copyTo(src);
	connect(ui.enter, SIGNAL(clicked()), this, SLOT(close()), Qt::UniqueConnection);
	connect(ui.exit, SIGNAL(clicked()), this, SLOT(send_mat()), Qt::UniqueConnection);


	connect(ui.constras_slider, SIGNAL(valueChanged(int)), ui.constras_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.constras_spinbox, SIGNAL(valueChanged(int)), ui.constras_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.constras_slider, SIGNAL(valueChanged(int)), this, SLOT(s_send()), Qt::UniqueConnection);

	connect(ui.bright_slider, SIGNAL(valueChanged(int)), ui.bright_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.bright_spinbox, SIGNAL(valueChanged(int)), ui.bright_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.bright_slider, SIGNAL(valueChanged(int)), this, SLOT(s_send()), Qt::UniqueConnection);

	connect(this, SIGNAL(send_val(double, double)), this, SLOT(s_contras(double, double)));

	this->update();

}


Image_brightAcontras::~Image_brightAcontras()
{}


void Image_brightAcontras::send_mat() {

	emit bright_and_constras_mat(src);
	this->close();

}

void Image_brightAcontras::s_send() {

	double beta = ui.bright_slider->value() / 100.0;
	double alpha = ui.constras_slider->value() / 100.0;
	emit send_val(beta,alpha);
}

void Image_brightAcontras::s_contras(double beta,double alpha) {
	cv::Mat dst;
	Image_contras_bright(src, dst, beta,alpha);
	emit bright_and_constras_mat(dst);
}