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