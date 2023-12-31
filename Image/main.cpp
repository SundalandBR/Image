#include "Image.h"
#include <QtWidgets/QApplication>
#include "Image_processing.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Image_processing w;
    w.show();
    return a.exec();
}


/*
#include "qimage.h"
int main(int argc, char* argv[])
{
    cv::Mat src = cv::imread("D:\\git\\Image\\test_image\\lena.png");

    if (src.empty()) {
        std::cerr << "Error: Unable to load image" << std::endl;
        return -1;
    }

    cv::Mat rgbMat;
    cv::cvtColor(src, rgbMat, cv::COLOR_BGR2RGB);
    QImage qmg(rgbMat.data, rgbMat.cols, rgbMat.rows, static_cast<int>(rgbMat.step), QImage::Format_RGB888);
    //qmg.save("D:\\git\\Image\\test_image\\lena1.png");
    cv::Mat mat(qmg.height(), qmg.width(), CV_8UC3, qmg.bits(), static_cast<size_t>(qmg.bytesPerLine()));

    cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
    QImage qmg1(rgbMat.data, rgbMat.cols, rgbMat.rows, static_cast<int>(rgbMat.step), QImage::Format_RGB888);
    cv::Mat mat1(qmg.height(), qmg.width(), CV_8UC3, qmg.bits(), static_cast<size_t>(qmg.bytesPerLine()));
    cv::imshow("Image", mat1);
    cv::waitKey(0);


    return 0;
}*/
