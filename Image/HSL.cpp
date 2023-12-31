#include "HSL.h"

extern cv::Mat Input;
HSL::HSL(QWidget *parent)
	: QWidget(parent)
{

	ui.setupUi(this);
	Input.copyTo(src);
	Input.copyTo(_dst);

	hsl_src = RGB2HSL(src);
	hsl_dst = RGB2HSL(src);

	/*
	ui.h_slider->setTracking(false);
	ui.l_slider->setTracking(false);
	ui.s_slider->setTracking(false);
	ui.h_slider_2->setTracking(false);
	ui.l_slider_2->setTracking(false);
	ui.s_slider_2->setTracking(false);
	ui.h_slider_3->setTracking(false);
	ui.l_slider_3->setTracking(false);
	ui.s_slider_3->setTracking(false);
	ui.h_slider_4->setTracking(false);
	ui.l_slider_4->setTracking(false);
	ui.s_slider_4->setTracking(false);
	ui.h_slider_5->setTracking(false);
	ui.l_slider_5->setTracking(false);
	ui.s_slider_5->setTracking(false);
	ui.h_slider_6->setTracking(false);
	ui.l_slider_6->setTracking(false);
	ui.s_slider_6->setTracking(false);
	*/

	connect(ui.h_slider, SIGNAL(valueChanged(int)), ui.h_box, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_box, SIGNAL(valueChanged(int)), ui.h_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_slider, SIGNAL(valueChanged(int)), this, SLOT(h_slider_changed()), Qt::UniqueConnection);
	connect(ui.l_slider, SIGNAL(valueChanged(int)), ui.l_box, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_box, SIGNAL(valueChanged(int)), ui.l_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_slider, SIGNAL(valueChanged(int)), this, SLOT(l_slider_changed()), Qt::UniqueConnection);
	connect(ui.s_slider, SIGNAL(valueChanged(int)), ui.s_box, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_box, SIGNAL(valueChanged(int)), ui.s_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_slider, SIGNAL(valueChanged(int)), this, SLOT(s_slider_changed()), Qt::UniqueConnection);

	connect(ui.h_slider_2, SIGNAL(valueChanged(int)), ui.h_box_2, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_box_2, SIGNAL(valueChanged(int)), ui.h_slider_2, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_slider_2, SIGNAL(valueChanged(int)), this, SLOT(h_slider2_changed()), Qt::UniqueConnection);
	connect(ui.l_slider_2, SIGNAL(valueChanged(int)), ui.l_box_2, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_box_2, SIGNAL(valueChanged(int)), ui.l_slider_2, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_slider_2, SIGNAL(valueChanged(int)), this, SLOT(l_slider2_changed()), Qt::UniqueConnection);
	connect(ui.s_slider_2, SIGNAL(valueChanged(int)), ui.s_box_2, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_box_2, SIGNAL(valueChanged(int)), ui.s_slider_2, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_slider_2, SIGNAL(valueChanged(int)), this, SLOT(s_slider2_changed()), Qt::UniqueConnection);

	connect(ui.h_slider_3, SIGNAL(valueChanged(int)), ui.h_box_3, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_box_3, SIGNAL(valueChanged(int)), ui.h_slider_3, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_slider_3, SIGNAL(valueChanged(int)), this, SLOT(h_slider3_changed()), Qt::UniqueConnection);
	connect(ui.l_slider_3, SIGNAL(valueChanged(int)), ui.l_box_3, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_box_3, SIGNAL(valueChanged(int)), ui.l_slider_3, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_slider_3, SIGNAL(valueChanged(int)), this, SLOT(l_slider3_changed()), Qt::UniqueConnection);
	connect(ui.s_slider_3, SIGNAL(valueChanged(int)), ui.s_box_3, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_box_3, SIGNAL(valueChanged(int)), ui.s_slider_3, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_slider_3, SIGNAL(valueChanged(int)), this, SLOT(s_slider3_changed()), Qt::UniqueConnection);


	connect(ui.h_slider_4, SIGNAL(valueChanged(int)), ui.h_box_4, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_box_4, SIGNAL(valueChanged(int)), ui.h_slider_4, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_slider_4, SIGNAL(valueChanged(int)), this, SLOT(h_slider4_changed()), Qt::UniqueConnection);
	connect(ui.l_slider_4, SIGNAL(valueChanged(int)), ui.l_box_4, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_box_4, SIGNAL(valueChanged(int)), ui.l_slider_4, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_slider_4, SIGNAL(valueChanged(int)), this, SLOT(l_slider4_changed()), Qt::UniqueConnection);
	connect(ui.s_slider_4, SIGNAL(valueChanged(int)), ui.s_box_4, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_box_4, SIGNAL(valueChanged(int)), ui.s_slider_4, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_slider_4, SIGNAL(valueChanged(int)), this, SLOT(s_slider4_changed()), Qt::UniqueConnection);


	connect(ui.h_slider_5, SIGNAL(valueChanged(int)), ui.h_box_5, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_box_5, SIGNAL(valueChanged(int)), ui.h_slider_5, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_slider_5, SIGNAL(valueChanged(int)), this, SLOT(h_slider5_changed()), Qt::UniqueConnection);
	connect(ui.l_slider_5, SIGNAL(valueChanged(int)), ui.l_box_5, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_box_5, SIGNAL(valueChanged(int)), ui.l_slider_5, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_slider_5, SIGNAL(valueChanged(int)), this, SLOT(l_slider5_changed()), Qt::UniqueConnection);
	connect(ui.s_slider_5, SIGNAL(valueChanged(int)), ui.s_box_5, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_box_5, SIGNAL(valueChanged(int)), ui.s_slider_5, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_slider_5, SIGNAL(valueChanged(int)), this, SLOT(s_slider5_changed()), Qt::UniqueConnection);


	connect(ui.h_slider_6, SIGNAL(valueChanged(int)), ui.h_box_6, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_box_6, SIGNAL(valueChanged(int)), ui.h_slider_6, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.h_slider_6, SIGNAL(valueChanged(int)), this, SLOT(h_slider6_changed()), Qt::UniqueConnection);
	connect(ui.l_slider_6, SIGNAL(valueChanged(int)), ui.l_box_6, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_box_6, SIGNAL(valueChanged(int)), ui.l_slider_6, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.l_slider_6, SIGNAL(valueChanged(int)), this, SLOT(l_slider6_changed()), Qt::UniqueConnection);
	connect(ui.s_slider_6, SIGNAL(valueChanged(int)), ui.s_box_6, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_box_6, SIGNAL(valueChanged(int)), ui.s_slider_6, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.s_slider_6, SIGNAL(valueChanged(int)), this, SLOT(s_slider6_changed()), Qt::UniqueConnection);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(exit()));
}

HSL::~HSL()
{}

void HSL::h_slider_changed() {

	Image_HSL(_dst, _dst, ui.h_slider->value(), RED, 1,hsl_src,hsl_dst);
	emit(send_mat(_dst));
}
void HSL::h_slider2_changed() {
	Image_HSL(_dst, _dst, ui.h_slider_2->value(), YELLOW, 1, hsl_src, hsl_dst);

	emit(send_mat(_dst));
}
void HSL::h_slider3_changed() {
	Image_HSL(_dst, _dst, ui.h_slider_3->value(), GREEN, 1, hsl_src, hsl_dst);
	emit(send_mat(_dst));
}
void HSL::h_slider4_changed() {
	Image_HSL(_dst, _dst, ui.h_slider_4->value(), BULEGREEN, 1, hsl_src, hsl_dst);

	emit(send_mat(_dst));
}
void HSL::h_slider5_changed() {
	Image_HSL(_dst, _dst, ui.h_slider_5->value() , BULE, 1, hsl_src, hsl_dst);
	
	emit(send_mat(_dst));
}
void HSL::h_slider6_changed() {
	Image_HSL(_dst, _dst, ui.h_slider_6->value() , MEGENTA, 1, hsl_src, hsl_dst);
	emit(send_mat(_dst));
}


void HSL::l_slider_changed() {
	Image_HSL(_dst, _dst, ui.l_slider->value() , RED, 2, hsl_src, hsl_dst);
	emit(send_mat(_dst));
}
void HSL::l_slider2_changed() {
	Image_HSL(_dst, _dst, ui.l_slider_2->value() , YELLOW, 2, hsl_src, hsl_dst);
	emit(send_mat(_dst));
}
void HSL::l_slider3_changed() {
	Image_HSL(_dst, _dst, ui.l_slider_3->value() , GREEN, 2, hsl_src, hsl_dst);
	emit(send_mat(_dst));
}
void HSL::l_slider4_changed() {
	Image_HSL(_dst, _dst, ui.l_slider_4->value() , BULEGREEN, 2, hsl_src, hsl_dst);
	emit(send_mat(_dst));
}
void HSL::l_slider5_changed() {
	Image_HSL(_dst, _dst, ui.l_slider_5->value(), BULE, 2, hsl_src, hsl_dst);
	emit(send_mat(_dst));
}
void HSL::l_slider6_changed() {
	Image_HSL(_dst, _dst, ui.l_slider_6->value(), MEGENTA, 2, hsl_src, hsl_dst);
	emit(send_mat(_dst));
}


void HSL::s_slider_changed() {
	Image_HSL(_dst, _dst, ui.s_slider->value(), RED, 3, hsl_src, hsl_dst);
	emit(send_mat(_dst));
}
void HSL::s_slider2_changed() {
	Image_HSL(_dst, _dst, ui.s_slider_2->value() , YELLOW, 3, hsl_src, hsl_dst);

	emit(send_mat(_dst));
}
void HSL::s_slider3_changed() {
	Image_HSL(_dst, _dst, ui.s_slider_3->value() , GREEN, 3, hsl_src, hsl_dst);

	emit(send_mat(_dst));
}
void HSL::s_slider4_changed() {
	Image_HSL(_dst, _dst, ui.s_slider_4->value() , BULEGREEN, 3, hsl_src, hsl_dst);

	emit(send_mat(_dst));
}
void HSL::s_slider5_changed() {
	Image_HSL(_dst, _dst, ui.s_slider_5->value() , BULE, 3, hsl_src, hsl_dst);

	emit(send_mat(_dst));
}
void HSL::s_slider6_changed() {
	Image_HSL(_dst, _dst, ui.s_slider_6->value() , MEGENTA, 3, hsl_src, hsl_dst);

	emit(send_mat(_dst));
}


void HSL::exit() {
	emit(send_mat(src));
	for (int i = 0; i < _dst.rows; i++) {
		delete[]hsl_src[i];
		delete[]hsl_dst[i];
	}
	delete[]hsl_src;
	delete[]hsl_dst;
	this->close();
}

void HSL::enter() {
	emit(send_mat(_dst));
	for (int i = 0; i < _dst.rows; i++) {
		delete[]hsl_src[i];
		delete[]hsl_dst[i];
	}
	delete[]hsl_src;
	delete[]hsl_dst;
	this->close();
}