#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Image.h"

class Image : public QMainWindow
{
    Q_OBJECT

public:
    Image(QWidget *parent = nullptr);
    ~Image();

private:
    Ui::ImageClass ui;
};
