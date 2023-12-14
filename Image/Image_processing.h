#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Image_processing.h"
#include "Image_opencv.h"



class Image_processing : public QMainWindow
{
    Q_OBJECT

public:
    Image_processing(QWidget *parent = nullptr);
    ~Image_processing();

private:
    Ui::Image_processingClass ui;
    double label_width_;
    double label_height_;
    bool init_label;

private slots:
    void on_ImgRead_menu_clicked();
    void on_Removal_button_clicked();
    void on_Rotate_button_clicked();
    void on_open_file_menu_clicked();
    void on_Crop_button_clicked();
    void on_B_and_C_button_clicked();
    void receive_mat(cv::Mat r_Mat);
    void on_curve_button_clicked();
    void on_text_button_clicked();

private:
    void update_photo_label();

};

class ip {
    cv::Mat org;
    cv::Mat dst;
};