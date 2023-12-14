#pragma once

#include <QMainWindow>
#include "ui_Image_Rotate_ui.h"
#include "Image_Rotate.h"

class Image_Rotate_ui : public QMainWindow
{
	Q_OBJECT

public:
	Image_Rotate_ui(QWidget *parent = nullptr);
	~Image_Rotate_ui();


private slots:
	void win_get_angel();
	void win_Image_Rotate(double angel,bool);
	void send_rotate_mat();
	void set_spainbox_value(int value);
	void set_slider_value(int value);
signals:
	void send_angel(double,bool);
	void send_rotate_mat(cv::Mat);

private:
	Ui::Image_Rotate_uiClass ui;
	virtual void paintEvent(QPaintEvent* event) override;

};
