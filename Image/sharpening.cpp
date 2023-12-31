#include "sharpening.h"

extern cv::Mat Input;
sharpening::sharpening(QWidget *parent)
	: QWidget(parent)
{
	Input.copyTo(src);
	sharpening_kernel_num = 0;
	ui.setupUi(this);
	connect(ui.sharp_comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(sharp_combox_value(QString)));
	connect(ui.sharpening_slider, SIGNAL(valueChanged(int)), ui.sharpening_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.sharpening_spinbox, SIGNAL(valueChanged(int)), ui.sharpening_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.sharpening_slider, SIGNAL(valueChanged(int)), this, SLOT(sharp_combox_slider_changed()), Qt::UniqueConnection);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(exit()));
}

sharpening::~sharpening()
{}

void sharpening::sharp_combox_slider_changed() {
	cv::Mat _dst;
	Image_sharpening(src, _dst, sharpening_kernel_num, ui.sharpening_slider->value());
	emit(send_mat(_dst));
}

void sharpening::sharp_combox_value(QString s) {
	if (s == "Laplacian") {
		sharpening_kernel_num = 0;
	}
	else if (s == "Sharpening") {
		sharpening_kernel_num = 1;
	}
	else if (s == "Laplacian++") {
		sharpening_kernel_num = 2;
	}
	else if (s == "Sharpening++") {
		sharpening_kernel_num = 3;
	}
	sharp_combox_slider_changed();
}


void sharpening::exit() {
	emit(send_mat(src));
	this->close();
}