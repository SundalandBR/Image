#include "Color_t.h"

extern cv::Mat Input;

Color_t::Color_t(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Input.copyTo(src);
	connect(ui.color_t_slider, SIGNAL(valueChanged(int)), ui.color_t_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.color_t_spinbox, SIGNAL(valueChanged(int)), ui.color_t_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.color_t_slider, SIGNAL(valueChanged(int)), this, SLOT(color_t_slider_changed()), Qt::UniqueConnection);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(exit()));
}

Color_t::~Color_t()
{}


void Color_t::color_t_slider_changed() {
	cv::Mat _dst;
	Image_ColorTemperature(src, _dst, ui.color_t_slider->value());
	emit(send_mat(_dst));
}

void Color_t::exit() {
	emit(send_mat(src));
	this->close();
}