#include "Image_processing.h"
#include "Image_Removal_ui.h"
#include "Image_Rotate_ui.h"
#include "Image_Crop.h"
#include "Image_brightAcontras.h"
#include "Qt_curve.h"
#include "Image_curve1.h"
#include "Image_curve2.h"
#include "Image_text.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qsize.h>

cv::Mat Input;
int row, col;

//�й�������������˵�����ʵ�ֶ��ڸ�cpp

Image_processing::Image_processing(QWidget *parent)
    : QMainWindow(parent){

    ui.setupUi(this);
	init_label = true;
	ui.Removal_button->setEnabled(false);
	ui.Rotate_button->setEnabled(false);
	ui.Photo_label->setScaledContents(true);
    connect(ui.Imgread_button, SIGNAL(clicked()), this, SLOT(on_ImgRead_menu_clicked()), Qt::UniqueConnection);
    connect(ui.Removal_button, SIGNAL(clicked()), this, SLOT(on_Removal_button_clicked()), Qt::UniqueConnection);
    connect(ui.open_file_menu, SIGNAL(triggered()), this, SLOT(on_ImgRead_menu_clicked()), Qt::UniqueConnection);
	connect(ui.B_and_C_button, SIGNAL(clicked()), this, SLOT(on_B_and_C_button_clicked()), Qt::UniqueConnection);
	connect(ui.curve1, SIGNAL(clicked()), this, SLOT(on_curve_button_clicked()), Qt::UniqueConnection);
}

Image_processing::~Image_processing()
{}


//�˵���_��ȡ�ļ�
void Image_processing::on_ImgRead_menu_clicked() {
	QString Fileadd = QFileDialog::getOpenFileName(this, tr("open"), "./", tr("Image Files (*.png;*.jpg;*.bmp)"));
	if (Fileadd.isEmpty()) {
		return;
	}
	Input = cv::imread(Fileadd.toLatin1().data());
	row = Input.rows;
	col = Input.cols;
	if (init_label) {
		label_height_ = ui.Photo_label->height();
		label_width_ = ui.Photo_label->width();
		init_label = false;
	}
	receive_mat(Input);
	ui.Removal_button->setEnabled(true);
	ui.Rotate_button->setEnabled(true);
	return;
}

void Image_processing::on_open_file_menu_clicked() {
	QMessageBox::information(this, tr("error"), tr("1111111111111111111111"));
	return;
}


/*
 * �����ڴ��Ӵ���
 * �Ӵ��ڽ��������ڵ�MAT
 * �Ӵ��ڷ��ʹ�����MAT��������
*/

/* ȥ�����Ӵ��� */
void Image_processing::on_Removal_button_clicked() {
	Image_Removal_ui* w;
	w = new Image_Removal_ui;
	w->show();
	connect(w, SIGNAL(send_removal_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat), Qt::UniqueConnection));
	return;
}

/* ��ת�����Ӵ��� */ 
void Image_processing::on_Rotate_button_clicked() {
	Image_Rotate_ui* w;
	w = new Image_Rotate_ui;
	w->show();
	connect(w, SIGNAL(send_rotate_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}

/* ��ͼ�����Ӵ���*/
void Image_processing::on_Crop_button_clicked() {
	Image_crop* w;
	w = new Image_crop;
	w->show();
	connect(w, SIGNAL(send_crop_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}



void Image_processing::on_text_button_clicked() {
	Image_text* w;
	w = new Image_text;
	w->show();
	//connect(w, SIGNAL(bright_and_constras_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}


void Image_processing::on_B_and_C_button_clicked() {
	Image_brightAcontras* w;
	w = new Image_brightAcontras;
	w->show();
	connect(w, SIGNAL(bright_and_constras_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}

void Image_processing::on_curve_button_clicked() {
	Image_curve2*w;
	w = new Image_curve2;
	w->show();
	connect(w, SIGNAL(signalsendmat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}


// ��ȡ�Ӵ��ڴ���õ�ͼ��
void Image_processing::receive_mat(cv::Mat r_Mat) {
	Input = r_Mat;
	row = Input.rows;
	col = Input.cols;
	cv::Mat tmat;
	Input.copyTo(tmat);
	cv::cvtColor(tmat, tmat, cv::COLOR_BGR2RGB);
	QImage Qinput = QImage((const unsigned char*)(tmat.data), tmat.cols, tmat.rows, tmat.step, QImage::Format_RGB888);
	float scale_w = label_width_ / Qinput.width();  //��ı���  
	float scale_h = label_height_ / Qinput.height();  //�ߵı���  
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
	qDebug() << ui.Photo_label->size();
	ui.Photo_label->show(QSize(new_width,new_height), QPixmap::fromImage(Qinput));
	ui.Photo_label->setAlignment(Qt::AlignCenter);
	return;
}