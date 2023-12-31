#pragma once
#pragma execution_character_set("UTF-8")


#include <QtWidgets/QMainWindow>
#include "ui_Image_processing.h"
#include "Image_opencv.h"
#include "Image_HSL.h"
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
#include "Image_exposure.h"
#include "Image_ColorTemperature.h"
#include "Image_sharpening.h"
#include "Image_blur.h"


#include "exposure.h"
#include "gamma.h"
#include "Color_t.h"
#include "HUE.h"
#include "saturation.h"
#include "sharpening.h"
#include "Qt_blur.h"
#include "Histogram.h"
#include "HSL.h"

#include "qmessagebox.h"



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
    int sharpening_kernel_num;
    bool init_label;

private slots:
    void on_ImgRead_menu_clicked();
    void on_Removal_menu_clicked();
    void on_Rotate_menu_clicked();
    //void on_open_file_menu_clicked();
    void on_Crop_menu_clicked();

    void on_curve_menu_clicked();
    void on_curve2_menu_clicked();
    void on_text_menu_clicked();

    void on_light_contras_menu_clicked();
    void on_exposure_menu_clicked();
    void on_gamma_menu_clicked();
    void on_colort_menu_clicked();
    void on_hue_menu_clicked();
    void on_saturation_menu_clicked();
    void on_sharpening_menu_clicked();
    void on_blur_menu_clicked();
    void on_histogram_menu_clicked();
    void on_HSL_menu_clicked();

    void receive_mat(cv::Mat r_Mat);

    //void on_save_button_clicked();
    //void on_histogram_button_clicked();
    //void on_HSL_buttn_clicked();


private:
    void update_photo_label();

signals:
    void main_show_mat(cv::Mat);


};

class ip {
    cv::Mat org;
    cv::Mat dst;
};