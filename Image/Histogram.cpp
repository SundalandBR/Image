#include "Histogram.h"

extern cv::Mat Input;

Histogram::Histogram(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(enter()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
	QTimer::singleShot(0, this, SLOT(his_fun()));
}

Histogram::~Histogram()
{}


void Histogram::his_fun() {
	label_width_ = ui.his_label->width();
	label_height_ = ui.his_label->height();
	Imgae_Histogram(Input, dst);
	print_image(ui.org_label, Input);
	print_image(ui.his_label, dst);
	return;
}

void Histogram::print_image(ViewLabel* tar, cv::Mat image) {
	cv::Mat tmat;
	image.copyTo(tmat);
	cv::cvtColor(tmat, tmat, cv::COLOR_BGR2RGB);
	QImage Qinput = QImage((const unsigned char*)(tmat.data), tmat.cols, tmat.rows, tmat.step, QImage::Format_RGB888);
	float scale_w = 1.f*label_width_ / Qinput.width();  //��ı���  
	float scale_h = 1.f*label_height_ / Qinput.height();  //�ߵı���  
	float new_width, new_height;  //�µĿ�͸�  
	if ((label_width_ >= Qinput.width()) && (label_height_ >= Qinput.height()))  //ͼƬ��ԭʼ��͸߾�С����ʾ�ؼ��Ŀ�͸�  
	{
		new_width = Qinput.width();
		new_height = Qinput.height();
	}
	else if (scale_w > scale_h)
	{
		new_width = Qinput.width() * scale_h;
		new_height = Qinput.height() * scale_h;
	}
	else if (scale_w <= scale_h)
	{
		new_width = Qinput.width() * scale_w;
		new_height = Qinput.height() * scale_w;
	}
	tar->show(QSize(new_width, new_height), QPixmap::fromImage(Qinput));
}

void Histogram::enter() {
	emit(send_mat(dst));
	this->close();
}