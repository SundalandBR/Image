#include "exposure.h"

extern cv::Mat Input;

exposure::exposure(QWidget *parent)
	: QWidget(parent)
{
	Input.copyTo(src);
	ui.setupUi(this);
	connect(ui.exposure_slider, SIGNAL(valueChanged(int)), ui.exposure_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.exposure_spinbox, SIGNAL(valueChanged(int)), ui.exposure_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.exposure_slider, SIGNAL(valueChanged(int)), this, SLOT(exposure_slider_changed()), Qt::UniqueConnection);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(exit()));
}

exposure::~exposure()
{}

void exposure::exposure_slider_changed() {
	cv::Mat _dst;
	Image_exposure(src, _dst, ui.exposure_slider->value());
	emit(send_mat(_dst));
}

void exposure::exit() {
	emit(send_mat(src));
	this->close();
}