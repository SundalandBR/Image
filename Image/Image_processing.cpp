#include "Image_processing.h"

cv::Mat Input;
cv::Mat _dst;
int row, col;

//有关主界面组件，菜单功能实现都在该cpp

Image_processing::Image_processing(QWidget *parent)
    : QMainWindow(parent){

    ui.setupUi(this);
	init_label = true;
	sharpening_kernel_num = 0;

	ui.Photo_label->setScaledContents(true);
	connect(ui.open_file_menu, SIGNAL(triggered()), this, SLOT(on_ImgRead_menu_clicked()), Qt::UniqueConnection);
    connect(ui.removal, SIGNAL(triggered()), this, SLOT(on_Removal_menu_clicked()), Qt::UniqueConnection);
	connect(ui.crop, SIGNAL(triggered()), this, SLOT(on_Crop_menu_clicked()), Qt::UniqueConnection);
	connect(ui.rotate, SIGNAL(triggered()), this, SLOT(on_Rotate_menu_clicked()), Qt::UniqueConnection);
	connect(ui.text, SIGNAL(triggered()), this, SLOT(on_text_menu_clicked()), Qt::UniqueConnection);
	connect(ui.light, SIGNAL(triggered()), this, SLOT(on_light_contras_menu_clicked()), Qt::UniqueConnection);
	connect(ui.curve1_, SIGNAL(triggered()), this, SLOT(on_curve_menu_clicked()), Qt::UniqueConnection);
	connect(ui.curve2_, SIGNAL(triggered()), this, SLOT(on_curve2_menu_clicked()), Qt::UniqueConnection);
	connect(ui.exposure, SIGNAL(triggered()), this, SLOT(on_exposure_menu_clicked()), Qt::UniqueConnection);
	connect(ui.gamma, SIGNAL(triggered()), this, SLOT(on_gamma_menu_clicked()), Qt::UniqueConnection);
	connect(ui.color_t, SIGNAL(triggered()), this, SLOT(on_colort_menu_clicked()), Qt::UniqueConnection);
	connect(ui.hue, SIGNAL(triggered()), this, SLOT(on_hue_menu_clicked()), Qt::UniqueConnection);
	connect(ui.saturation, SIGNAL(triggered()), this, SLOT(on_saturation_menu_clicked()), Qt::UniqueConnection);
	connect(ui.sharpening, SIGNAL(triggered()), this, SLOT(on_sharpening_menu_clicked()), Qt::UniqueConnection);
	connect(ui.blur, SIGNAL(triggered()), this, SLOT(on_blur_menu_clicked()), Qt::UniqueConnection);
	connect(ui.histogram, SIGNAL(triggered()), this, SLOT(on_histogram_menu_clicked()), Qt::UniqueConnection);
	connect(ui.HSL, SIGNAL(triggered()), this, SLOT(on_HSL_menu_clicked()), Qt::UniqueConnection);
	connect(this, SIGNAL(main_show_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)));

}

Image_processing::~Image_processing()
{}


//菜单栏_读取文件
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
	return;
}


/*
 * 主窗口打开子窗口
 * 子窗口接收主窗口的MAT
 * 子窗口发送处理后的MAT给主窗口
*/

/* 去雾功能子窗口 */
void Image_processing::on_Removal_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes,QMessageBox::Yes);
		return;
	}
	Image_Removal_ui* w;
	w = new Image_Removal_ui;
	w->show();
	connect(w, SIGNAL(send_removal_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat), Qt::UniqueConnection));
	return;
}

/* 旋转功能子窗口 */ 
void Image_processing::on_Rotate_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	Image_Rotate_ui* w;
	w = new Image_Rotate_ui;
	w->show();
	connect(w, SIGNAL(send_rotate_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}

/* 构图功能子窗口*/
void Image_processing::on_Crop_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	Image_crop* w;
	w = new Image_crop;
	w->show();
	connect(w, SIGNAL(send_crop_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}



void Image_processing::on_text_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	Image_text* w;
	w = new Image_text;
	w->show();
	connect(w, SIGNAL(send_text_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}


void Image_processing::on_light_contras_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	Image_brightAcontras* w;
	w = new Image_brightAcontras;
	w->show();
	connect(w, SIGNAL(bright_and_constras_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}

void Image_processing::on_curve_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	Image_curve1*w;
	w = new Image_curve1;
	w->show();
	connect(w, SIGNAL(signalsendmat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}

void Image_processing::on_curve2_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	Image_curve2* w;
	w = new Image_curve2;
	w->show();
	connect(w, SIGNAL(signalsendmat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}

// 读取子窗口处理好的图像
void Image_processing::receive_mat(cv::Mat r_Mat) {
	r_Mat.copyTo(Input);
	row = Input.rows;
	col = Input.cols;
	cv::Mat tmat;
	Input.copyTo(tmat);
	cv::cvtColor(tmat, tmat, cv::COLOR_BGR2RGB);
	QImage Qinput = QImage((const unsigned char*)(tmat.data), tmat.cols, tmat.rows, tmat.step, QImage::Format_RGB888);
	float scale_w = label_width_ / Qinput.width();  //宽的比例  
	float scale_h = label_height_ / Qinput.height();  //高的比例  
	float new_width, new_height;  //新的宽和高  
	if ((label_width_ >= Qinput.width()) && (label_height_ >= Qinput.height()))  //图片的原始宽和高均小于显示控件的宽和高  
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

void Image_processing::on_exposure_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	exposure* w;
	w = new exposure;
	w->show();
	connect(w, SIGNAL(send_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}
void Image_processing::on_gamma_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	gamma* w;
	w = new gamma;
	w->show();
	connect(w, SIGNAL(send_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}
void Image_processing::on_colort_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	Color_t* w;
	w = new Color_t;
	w->show();
	connect(w, SIGNAL(send_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}
void Image_processing::on_hue_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	Color_t* w;
	w = new Color_t;
	w->show();
	connect(w, SIGNAL(send_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}
void Image_processing::on_saturation_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	saturation* w;
	w = new saturation;
	w->show();
	connect(w, SIGNAL(send_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}
void Image_processing::on_sharpening_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	sharpening* w;
	w = new sharpening;
	w->show();
	connect(w, SIGNAL(send_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}
void Image_processing::on_blur_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	Qt_blur* w;
	w = new Qt_blur;
	w->show();
	connect(w, SIGNAL(send_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}
void Image_processing::on_histogram_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	Histogram* w;
	w = new Histogram;
	w->show();
	connect(w, SIGNAL(send_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}
void Image_processing::on_HSL_menu_clicked() {
	if (Input.empty()) {
		QMessageBox::information(NULL, "ERROR", QString::fromLocal8Bit("未选择图片"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	HSL* w;
	w = new HSL;
	w->show();
	connect(w, SIGNAL(send_mat(cv::Mat)), this, SLOT(receive_mat(cv::Mat)), Qt::UniqueConnection);
	return;
}