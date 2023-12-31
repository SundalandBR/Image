#include "blur.h"

extern cv::Mat Input;

blur::blur(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Input.copyTo(src);
	connect(ui.blur_slider, SIGNAL(valueChanged(int)), ui.blur_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.blur_spinbox, SIGNAL(valueChanged(int)), ui.blur_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.blur_slider, SIGNAL(valueChanged(int)), this, SLOT(blur_slider_changed()), Qt::UniqueConnection);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_2,SIGNAL(clicked()), this, SLOT(exit()));
}

blur::~blur()
{}


void blur::blur_slider_changed() {
	cv::Mat _dst;
	Image_blur(src, _dst, ui.blur_slider->value(), 0.3);
	emit(send_mat(_dst));
}

void blur::exit() {
	emit(send_mat(src));
	this->close();
}