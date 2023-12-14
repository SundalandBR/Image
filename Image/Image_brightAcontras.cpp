#include "Image_brightAcontras.h"

extern cv::Mat Input;
static cv::Mat Output;


Image_brightAcontras::Image_brightAcontras(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	Input.copyTo(Output);

	qDebug() << ui.label->size();

	connect(ui.exit, SIGNAL(clicked()), this, SLOT(close()), Qt::UniqueConnection);
	connect(ui.enter, SIGNAL(clicked()), this, SLOT(send_mat()), Qt::UniqueConnection);


	connect(ui.constras_slider, SIGNAL(valueChanged(int)), ui.constras_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.constras_spinbox, SIGNAL(valueChanged(int)), ui.constras_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.constras_slider, SIGNAL(valueChanged(int)), this, SLOT(s_send()), Qt::UniqueConnection);

	connect(ui.bright_slider, SIGNAL(valueChanged(int)), ui.bright_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.bright_spinbox, SIGNAL(valueChanged(int)), ui.bright_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.bright_slider, SIGNAL(valueChanged(int)), this, SLOT(s_send()), Qt::UniqueConnection);

	connect(ui.gamma_slider, SIGNAL(valueChanged(int)), ui.gamma_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.gamma_spinbox, SIGNAL(valueChanged(int)), ui.gamma_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.gamma_slider, SIGNAL(valueChanged(int)), this, SLOT(s_send()), Qt::UniqueConnection);
	connect(this, SIGNAL(send_val(double, double, double)), this, SLOT(s_contras(double, double, double)));

	this->update();

}


void Image_brightAcontras::paintEvent(QPaintEvent* event) {

	ui.label->setAlignment(Qt::AlignCenter);
	cv::Mat tmat;
	Output.copyTo(tmat);
	cv::cvtColor(tmat, tmat, cv::COLOR_BGR2RGB);
	QImage Qinput = QImage((const unsigned char*)(tmat.data), tmat.cols, tmat.rows, tmat.step, QImage::Format_RGB888);
	ui.label->setPixmap(QPixmap::fromImage(Qinput.scaled(ui.label->size(), Qt::KeepAspectRatio)));

}


Image_brightAcontras::~Image_brightAcontras()
{}


void Image_brightAcontras::send_mat() {

	emit bright_and_constras_mat(Output);
	this->close();

}


void Image_brightAcontras::s_send() {

	double beta = ui.bright_slider->value() / 100.0;
	double alpha = ui.constras_slider->value() / 100.0;
	double gamma = ui.gamma_slider->value() / 10.0;

	emit send_val(beta,alpha,gamma);
}

void Image_brightAcontras::s_contras(double beta,double alpha,double gamma) {
	Image_contras_bright(Input, Output, beta,alpha);
	Image_contras_gamma(Output, Output, gamma);
	this->update();
}

void Image_brightAcontras::update_label(cv::Mat Output) {
	cv::Mat tmat;
	Output.copyTo(tmat);
	cv::cvtColor(tmat, tmat, cv::COLOR_BGR2RGB);
	QImage Qinput = QImage((const unsigned char*)(tmat.data), tmat.cols, tmat.rows, tmat.step, QImage::Format_RGB888);
	ui.label->setPixmap(QPixmap::fromImage(Qinput.scaled(ui.label->size(), Qt::KeepAspectRatio)));
	qDebug() << ui.label->size();
}