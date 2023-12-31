#include "Image_Removal_ui.h"
#include "Image_Removal.h"
#include "QMessageBox"


extern cv::Mat Input;
static cv::Mat removal_mat;

Image_Removal_ui::Image_Removal_ui(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);

	Input.copyTo(removal_mat);

	ui.reg_silder->setDisabled(true);
	ui.reg_spinbox->setDisabled(true);
	ui.gdradius_silder->setDisabled(true);
	ui.gdradius_spinbox->setDisabled(true);

	connect(ui.radius_slider, SIGNAL(valueChanged(int)), ui.radius_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.radius_spinbox, SIGNAL(valueChanged(int)), ui.radius_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.radius_slider, SIGNAL(valueChanged(int)), this,SLOT(send_removal_val_from_ui()), Qt::UniqueConnection);

	connect(ui.omega_slider, SIGNAL(valueChanged(int)), ui.omega_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.omega_spinbox, SIGNAL(valueChanged(int)), ui.omega_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.omega_spinbox, SIGNAL(valueChanged(int)), this, SLOT(send_removal_val_from_ui()), Qt::UniqueConnection);

	connect(ui.GD_cbox, SIGNAL(clicked()), this, SLOT(open_gdval()), Qt::UniqueConnection);
	connect(ui.GD_cbox, SIGNAL(clicked()), this, SLOT(send_removal_val_from_ui()), Qt::UniqueConnection);

	connect(ui.gdradius_silder, SIGNAL(valueChanged(int)), ui.gdradius_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.gdradius_spinbox, SIGNAL(valueChanged(int)), ui.gdradius_silder, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.gdradius_silder, SIGNAL(valueChanged(int)), this, SLOT(send_removal_val_from_ui()), Qt::UniqueConnection);

	connect(ui.reg_silder, SIGNAL(valueChanged(int)), ui.reg_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.reg_spinbox, SIGNAL(valueChanged(int)), ui.reg_silder, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.reg_silder, SIGNAL(valueChanged(int)), this, SLOT(send_removal_val_from_ui()), Qt::UniqueConnection);

	connect(ui.t0_silder, SIGNAL(valueChanged(int)), ui.t0_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.t0_spinbox, SIGNAL(valueChanged(int)), ui.t0_silder, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.t0_silder, SIGNAL(valueChanged(int)), this, SLOT(send_removal_val_from_ui()), Qt::UniqueConnection);

	connect(ui.Enter_button, SIGNAL(clicked()), this, SLOT(on_enter_button_clicked()));
	connect(ui.exit_button, SIGNAL(clicked()), this, SLOT(close()));

	connect(this, SIGNAL(send_removal_val(int, double, bool, int, double, double)), this, SLOT(IMAGE_REMOVAL(int, double, bool, int, double, double), Qt::UniqueConnection));
	
	this->update();

}

Image_Removal_ui::~Image_Removal_ui()
{}

void Image_Removal_ui::on_enter_button_clicked() {
	emit send_removal_mat(removal_mat);
	this->close();
}

void Image_Removal_ui::paintEvent(QPaintEvent* event) {

	qDebug() << "1";
	cv::Mat tmat;
	removal_mat.copyTo(tmat);
	cv::cvtColor(tmat, tmat, cv::COLOR_BGR2RGB);
	QImage Qinput = QImage((const unsigned char*)(tmat.data), tmat.cols, tmat.rows, tmat.step, QImage::Format_RGB888);
	ui.removal_label->setPixmap(QPixmap::fromImage(Qinput.scaled(ui.removal_label->size(), Qt::KeepAspectRatio)));
	ui.removal_label->setAlignment(Qt::AlignCenter);

}


void Image_Removal_ui::IMAGE_REMOVAL(int radius,double omega,bool GD,int G_radius,double reg,double t0) {

	cv::Mat output = Imgae_Removal(Input, radius, omega, GD, G_radius, reg, t0);
	removal_mat = output;
	this->update();
}


void Image_Removal_ui::send_removal_val_from_ui() {
	int radius = int(ui.radius_slider->value());
	double omega = (double)(ui.omega_slider->value())/100.0;
	bool GD = ui.GD_cbox->isChecked();
	int G_radius = int(ui.gdradius_silder->value());
	double reg = (double)(ui.reg_silder->value()) / 10;
	double t0 = (double)(ui.t0_silder->value())/10.0;
	qDebug() << radius << omega << GD << G_radius << reg << t0;
	emit send_removal_val(radius, omega, GD, G_radius, reg*reg, t0);
}

void Image_Removal_ui::open_gdval() {
	bool gd = ui.GD_cbox->isChecked();
	ui.reg_silder->setDisabled(!gd);
	ui.reg_spinbox->setDisabled(!gd);
	ui.gdradius_silder->setDisabled(!gd);
	ui.gdradius_spinbox->setDisabled(!gd);
}