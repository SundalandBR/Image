#include "gamma.h"

extern cv::Mat Input;
gamma::gamma(QWidget *parent)
	: QWidget(parent)
{
	Input.copyTo(src);
	ui.setupUi(this);
	connect(ui.gamma_slider, SIGNAL(valueChanged(int)), ui.gamma_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.gamma_spinbox, SIGNAL(valueChanged(int)), ui.gamma_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.gamma_slider, SIGNAL(valueChanged(int)), this, SLOT(gamma_slider_changed()), Qt::UniqueConnection);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(exit()));
}

gamma::~gamma()
{}


void gamma::gamma_slider_changed() {
	cv::Mat _dst;
	int gamma = -ui.gamma_slider->value();
	double base = gamma >= 0 ? 1 : 0;
	double offset = gamma >= 0 ? gamma * 9 / 100 :1 - (double)abs(gamma) / 100.f;
	qDebug() << gamma << base << offset;
	Image_contras_gamma(src, _dst, base + offset);
	emit(send_mat(_dst));
}

void gamma::exit() {
	emit(send_mat(src));
	this->close();
}