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
	ui.gdredius_silder->setDisabled(true);
	ui.gdredius_spinbox->setDisabled(true);

	connect(ui.redius_slider, SIGNAL(valueChanged(int)), ui.redius_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.redius_spinbox, SIGNAL(valueChanged(int)), ui.redius_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.redius_slider, SIGNAL(valueChanged(int)), this,SLOT(send_removal_val_from_ui()), Qt::UniqueConnection);

	connect(ui.omega_slider, SIGNAL(valueChanged(int)), ui.omega_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.omega_spinbox, SIGNAL(valueChanged(int)), ui.omega_slider, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.omega_spinbox, SIGNAL(valueChanged(int)), this, SLOT(send_removal_val_from_ui()), Qt::UniqueConnection);

	connect(ui.GD_cbox, SIGNAL(clicked()), this, SLOT(open_gdval()), Qt::UniqueConnection);
	connect(ui.GD_cbox, SIGNAL(clicked()), this, SLOT(send_removal_val_from_ui()), Qt::UniqueConnection);

	connect(ui.gdredius_silder, SIGNAL(valueChanged(int)), ui.gdredius_spinbox, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.gdredius_spinbox, SIGNAL(valueChanged(int)), ui.gdredius_silder, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(ui.gdredius_silder, SIGNAL(valueChanged(int)), this, SLOT(send_removal_val_from_ui()), Qt::UniqueConnection);

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

	cv::Mat tmat;
	removal_mat.copyTo(tmat);
	cv::cvtColor(tmat, tmat, cv::COLOR_BGR2RGB);
	QImage Qinput = QImage((const unsigned char*)(tmat.data), tmat.cols, tmat.rows, tmat.step, QImage::Format_RGB888);
	ui.removal_label->setPixmap(QPixmap::fromImage(Qinput.scaled(ui.removal_label->size(), Qt::KeepAspectRatio)));
	ui.removal_label->setAlignment(Qt::AlignCenter);

}


void Image_Removal_ui::IMAGE_REMOVAL(int redius,double omega,bool GD,int G_redius,double reg,double t0) {

	cv::Mat output = Imgae_Removal(Input, redius, omega, GD, G_redius, reg, t0);
	removal_mat = output;
	this->update();
}


void Image_Removal_ui::send_removal_val_from_ui() {
	int redius = int(ui.redius_slider->value());
	double omega = (double)(ui.omega_slider->value())/100.0;
	bool GD = ui.GD_cbox->isChecked();
	int G_redius = int(ui.gdredius_silder->value());
	double reg = (double)(ui.reg_silder->value()) / 10;
	double t0 = (double)(ui.t0_silder->value())/10.0;
	qDebug() << redius << omega << GD << G_redius << reg << t0;
	emit send_removal_val(redius, omega, GD, G_redius, reg*reg, t0);
}

void Image_Removal_ui::open_gdval() {
	bool gd = ui.GD_cbox->isChecked();
	ui.reg_silder->setDisabled(!gd);
	ui.reg_spinbox->setDisabled(!gd);
	ui.gdredius_silder->setDisabled(!gd);
	ui.gdredius_spinbox->setDisabled(!gd);
}