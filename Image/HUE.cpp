#include "HUE.h"
extern cv::Mat Input;
HUE::HUE(QWidget *parent)
	: QWidget(parent)
{
	Input.copyTo(src);
	ui.setupUi(this);
	connect(ui.hue_slider, SIGNAL(valueChanged(int)), ui.hue_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.hue_spinbox, SIGNAL(valueChanged(int)), ui.hue_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.hue_slider, SIGNAL(valueChanged(int)), this, SLOT(hue_slider_changed()), Qt::UniqueConnection);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(exit()));
}

HUE::~HUE()
{}

void HUE::hue_slider_changed() {
	cv::Mat _dst;
	Image_HUE(src, _dst, ui.hue_slider->value());
	emit(send_mat(_dst));
}

void HUE::exit() {
	emit(send_mat(src));
	this->close();
}