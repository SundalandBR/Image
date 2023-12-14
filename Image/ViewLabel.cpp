#include "ViewLabel.h"

ViewLabel::ViewLabel(QWidget* parent)
    : QLabel(parent)
{}

ViewLabel::~ViewLabel()
{}

void ViewLabel::show(const QRect& rect, const QPixmap& pixmap)
{
    this->setGeometry(rect);
    if (!pixmap.isNull()) {
        int w = rect.width();
        int h = rect.height();
        this->setPixmap(pixmap.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}


void ViewLabel::show(int x, int y, int w, int h, const QPixmap& pixmap)
{
    QRect rect(x, y, w, h);
    ViewLabel::show(rect, pixmap);
}

void ViewLabel::show(const QSize& size, const QPixmap& pixmap)
{
    this->setFixedSize(size);

    if (!pixmap.isNull()) {
        int w = size.width();
        int h = size.height();
        this->setPixmap(pixmap.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

}