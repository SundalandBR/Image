#include "saturation.h"


extern cv::Mat Input;

saturation::saturation(QWidget *parent)
	: QWidget(parent)
{
	Input.copyTo(src);
	ui.setupUi(this);
	connect(ui.saturation_slider, SIGNAL(valueChanged(int)), ui.saturation_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.saturation_spinbox, SIGNAL(valueChanged(int)), ui.saturation_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.saturation_slider, SIGNAL(valueChanged(int)), this, SLOT(saturation_slider_changed()), Qt::UniqueConnection);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(exit()));
}

saturation::~saturation()
{}


void saturation::saturation_slider_changed() {
	cv::Mat _dst;
	Image_Saturation(src, _dst, ui.saturation_slider->value());
	emit(send_mat(_dst));
}

void saturation::exit() {
	emit(send_mat(src));
	this->close();
}