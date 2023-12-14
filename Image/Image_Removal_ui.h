#pragma once

#include <QMainWindow>
#include "ui_Image_Removal_ui.h"
#include "Image_opencv.h"




class Image_Removal_ui : public QMainWindow
{
	Q_OBJECT

public:
	Image_Removal_ui(QWidget *parent = nullptr);
	~Image_Removal_ui();


private:
	Ui::Image_Removal_uiClass ui;
	virtual void paintEvent(QPaintEvent* event) override;

private slots:
	void IMAGE_REMOVAL(int redius, double omega, bool GD, int G_redius, double reg, double t0);
	void on_enter_button_clicked();
	void send_removal_val_from_ui();
	void open_gdval();

signals:
	void send_removal_mat(cv::Mat Output);
	void send_removal_val(int redius, double omega, bool GD, int G_redius, double reg, double t0);



};
