#include "Image_Rotate_ui.h"

extern cv::Mat Input;
static cv::Mat Output;

Image_Rotate_ui::Image_Rotate_ui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	Input.copyTo(Output);
	connect(ui.rotate_spinBox, SIGNAL(valueChanged(int)), this, SLOT(set_slider_value(int)),Qt::UniqueConnection);
	connect(ui.rotate_Slider, SIGNAL(valueChanged(int)), this, SLOT(set_spainbox_value(int)),Qt::UniqueConnection);
	connect(ui.rotate_Slider, SIGNAL(valueChanged(int)), this, SLOT(win_get_angel()),Qt::UniqueConnection);
	connect(ui.black_button, SIGNAL(clicked()), this, SLOT(win_get_angel(), Qt::UniqueConnection));
	connect(this, SIGNAL(send_angel(double,bool)), this, SLOT(win_Image_Rotate(double,bool), Qt::UniqueConnection));
	connect(ui.Enter_button, SIGNAL(clicked()), this, SLOT(send_rotate_mat()), Qt::UniqueConnection);
	connect(ui.Exit_buttron, SIGNAL(clicked()), this, SLOT(close()), Qt::UniqueConnection);
	this->update();
}


Image_Rotate_ui::~Image_Rotate_ui()
{}


void Image_Rotate_ui::paintEvent(QPaintEvent* event) {
	cv::Mat tmat;
	Output.copyTo(tmat);
	cv::cvtColor(tmat, tmat, cv::COLOR_BGR2RGB);
	QImage Qinput = QImage((const unsigned char*)(tmat.data), tmat.cols, tmat.rows, tmat.step, QImage::Format_RGB888);
	ui.rotate_label->setPixmap(QPixmap::fromImage(Qinput.scaled(ui.rotate_label->size(), Qt::KeepAspectRatio)));
	ui.rotate_label->setAlignment(Qt::AlignCenter);
}

//Í¼ÏñÐý×ª
void Image_Rotate_ui::win_Image_Rotate(double angle,bool black) {
	Image_Rotate(Input, Output, angle,black);
	this->update();
}

void Image_Rotate_ui::win_get_angel() {
	double angle = double(ui.rotate_spinBox->value());
	bool black = ui.black_button->isChecked();
	emit send_angel(angle,black);
}

void Image_Rotate_ui::set_spainbox_value(int value) {

	ui.rotate_spinBox->setValue(value - 45);

}

void Image_Rotate_ui::set_slider_value(int value) {

	ui.rotate_Slider->setValue(value + 45);

}

void Image_Rotate_ui::send_rotate_mat() {
	emit send_rotate_mat(Output);
	this->close();
}