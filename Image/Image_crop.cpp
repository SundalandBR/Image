#include "Image_crop.h"


extern cv::Mat Input;
static cv::Mat Output;

Image_crop::Image_crop(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    CONSTRUCTED_FUNCTION = true;

    //区域内外等颜色值
    outsideColor_ = QColor(200, 200, 200, 150);
    //outsideColor_ = QColor(255, 0, 0, 255);
    insideColor_ = QColor(200, 200, 200, 0);
    dashColor_ = QColor(30, 162, 255, 255);

    //几个可以调整的点
    labelLeftBottom_ = new QLabel(this);
    labelLeftBottom_->setObjectName(QString::fromUtf8("labelLeftBottom"));
    labelLeftBottom_->setFixedSize(sDragDotWidth_, sDragDotWidth_);
    labelLeftBottom_->setCursor(QCursor(Qt::SizeBDiagCursor));
    labelLeftBottom_->setStyleSheet(QString::fromUtf8("background-color: rgb(30,162,255);"));
    labelRightBottom_ = new QLabel(this);
    labelRightBottom_->setObjectName(QString::fromUtf8("labelRightBottom"));
    labelRightBottom_->setFixedSize(sDragDotWidth_, sDragDotWidth_);
    labelRightBottom_->setCursor(QCursor(Qt::SizeFDiagCursor));
    labelRightBottom_->setStyleSheet(QString::fromUtf8("background-color: rgb(30,162,255);"));
    labelLeftTop_ = new QLabel(this);
    labelLeftTop_->setObjectName(QString::fromUtf8("labelLeftTop"));
    labelLeftTop_->setFixedSize(sDragDotWidth_, sDragDotWidth_);
    labelLeftTop_->setCursor(QCursor(Qt::SizeFDiagCursor));
    labelLeftTop_->setStyleSheet(QString::fromUtf8("background-color: rgb(30,162,255);"));
    labelRightTop_ = new QLabel(this);
    labelRightTop_->setObjectName(QString::fromUtf8("labelRightTop"));
    labelRightTop_->setFixedSize(sDragDotWidth_, sDragDotWidth_);
    labelRightTop_->setCursor(QCursor(Qt::SizeBDiagCursor));
    labelRightTop_->setStyleSheet(QString::fromUtf8("background-color: rgb(30,162,255);"));
    labelLeftBottom_->installEventFilter(this);
    labelRightBottom_->installEventFilter(this);
    labelLeftTop_->installEventFilter(this);
    labelRightTop_->installEventFilter(this);
    this->update();
    connect(this, SIGNAL(send_ratio(float)), this, SLOT(setFixCenterRectRatio(float)));
    connect(ui.crop_combobox, SIGNAL(currentTextChanged(QString)), this, SLOT(choose_ratio(QString)));
    connect(ui.enter_button, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui.enter_button, SIGNAL(clicked()), this, SLOT(send_crop_mat()));
    connect(ui.exit_button, SIGNAL(clicked()), this, SLOT(close()));
}



Image_crop::~Image_crop()
{}

int Image_crop::sDragDotWidth_ = 6;
int Image_crop::sDragDotOffset_ = 2;
int Image_crop::sCenterRectMinWidth_ = 6;
int Image_crop::sLineWidth_ = 1;


void Image_crop::after_image_crop() {
    cv::Mat tmat;
    Input.copyTo(tmat);
    cv::cvtColor(tmat, tmat, cv::COLOR_BGR2RGB);
    QImage Qinput = QImage((const unsigned char*)(tmat.data), tmat.cols, tmat.rows, tmat.step, QImage::Format_RGB888);
    ui.crop_label->resize(tmat.size().width, tmat.size().height);
    pix = QPixmap::fromImage(Qinput.scaled(ui.crop_label->size()));
    labelWidth_ = ui.crop_label->width();
    labelHeigth_ = ui.crop_label->height();
    labelX_ = ui.crop_label->pos().x() - cvRound(labelWidth_ / 2);
    labelY_ = ui.crop_label->pos().y() - cvRound(labelHeigth_ / 2);
    centerRect_.setX(labelX_);
    centerRect_.setY(labelY_);
    centerRect_.setWidth(labelWidth_);
    centerRect_.setHeight(labelHeigth_);
    return;
}

void Image_crop::choose_ratio(QString s) {
    fix_crop = true;
    float ratio = 0;
    if (s == "2:3") {
        ratio = 2.0 / 3;
    }
    else if (s == "3:2") {
        ratio = 3.0 / 2;
    }
    else if (s == "4:3") {
        ratio = 4.0 / 3;
    }
    else if (s == "3:4") {
        ratio = 3.0 / 4;
    }
    else if (s == "16:9") {
        ratio = 3.0 / 4;
    }
    else if (s == "9:16") {
        ratio = 9.0 / 16;
    }
    else if (s.compare(QString::fromLocal8Bit("原比例")) == 0) {
        ratio = 1.0 * labelWidth_ / labelHeigth_;
    }
    else if (s.compare(QString::fromLocal8Bit("自由")) == 0) {
        fix_crop = false;
    }

    emit send_ratio(ratio);
}



void Image_crop::setFixCenterRectRatio(float ratio) {
    /*
    * 按当前框大小 重新更改长 / 宽比例
    * 如果ratio > 1 固定长为图片的长度 修改宽度
    * 如果ratio < 1 固定宽为图片的宽度 修改长度
    * x y 不变
    */
    if (fix_crop) {
        fixCenterRectRatio_ = ratio;
        float widthBHeight = labelWidth_ * 1.0f / labelHeigth_;
        int tmpWidth = 0;
        int tmpHeight = 0;
        if (widthBHeight > ratio) {
            tmpHeight = labelHeigth_;
            tmpWidth = ratio * tmpHeight;
        }
        else {
            tmpWidth = labelWidth_;
            tmpHeight = tmpWidth / ratio;
        }

        //设置选择框值
        centerRect_.setRect(labelX_, labelY_, tmpWidth, tmpHeight);
    }
    this->update();

}

//中心矩阵合法性判断
void Image_crop::check_centerRect() {

    // labelx <= center.x <= labelx + width
    // labelx + center.width <= center.x  + center.width <= labelx + width

    // labely <= center.y <= labely + height
    // labely + center.heigth < center.y + center.height <= labely + height



    if (centerRect_.top() < labelY_) {
        centerRect_.setTop(labelY_);
    }
    if (centerRect_.bottom() > labelY_ + labelHeigth_ - 1) {
        centerRect_.setBottom(labelY_ + labelHeigth_ - 1);
    }
    if (centerRect_.left() < labelX_) {
        centerRect_.setLeft(labelX_);
    }
    if (centerRect_.right() > labelX_ + labelWidth_ - 1) {
        centerRect_.setRight(labelX_ + labelWidth_ - 1);
    }


}

void Image_crop::setRects() {

    leftRect_.setRect(labelX_, labelY_, centerRect_.x() - labelX_, labelHeigth_);
    topRect_.setRect(centerRect_.x(), labelY_, centerRect_.width() + sLineWidth_, centerRect_.y() - labelY_);
    rightRect_.setRect(centerRect_.width() + centerRect_.x() + sLineWidth_, labelY_, labelX_ + labelWidth_ - centerRect_.width() - centerRect_.x() + 1, labelHeigth_);
    bottomRect_.setRect(centerRect_.x(), centerRect_.height() + centerRect_.y() + sLineWidth_, centerRect_.width() + sLineWidth_, labelY_ + labelHeigth_ - centerRect_.height() - centerRect_.y() - 1);


}

//update() 调用
void Image_crop::paintEvent(QPaintEvent* event)
{


    if (CONSTRUCTED_FUNCTION) {
        CONSTRUCTED_FUNCTION = false;
        after_image_crop();
    }

    check_centerRect();
    setRects();
    QPainter painter(this);
    painter.drawPixmap(labelX_, labelY_, labelWidth_, labelHeigth_, pix);
    //绘制区域
    painter.fillRect(leftRect_, outsideColor_);
    painter.fillRect(topRect_, outsideColor_);
    painter.fillRect(rightRect_, outsideColor_);
    painter.fillRect(bottomRect_, outsideColor_);

    //绘制中间区域和边框虚线

    dashPen_.setBrush(QBrush(dashColor_));
    painter.setPen(dashPen_);
    painter.fillRect(centerRect_, insideColor_);
    painter.drawRect(centerRect_);

    //move zoomdot
    labelLeftBottom_->move(centerRect_.x() - sDragDotWidth_ / 2 + sDragDotOffset_, centerRect_.y() + centerRect_.height() - sDragDotWidth_ / 2 - sDragDotOffset_);
    labelRightBottom_->move(centerRect_.x() + centerRect_.width() - sDragDotWidth_ / 2 - sDragDotOffset_, centerRect_.y() + centerRect_.height() - sDragDotWidth_ / 2 - sDragDotOffset_);
    labelLeftTop_->move(centerRect_.x() - sDragDotWidth_ / 2 + sDragDotOffset_, centerRect_.y() - sDragDotWidth_ / 2 + sDragDotOffset_);
    labelRightTop_->move(centerRect_.x() + centerRect_.width() - sDragDotWidth_ / 2 - sDragDotOffset_, centerRect_.y() - sDragDotWidth_ / 2 + sDragDotOffset_);



}

void Image_crop::send_crop_mat() {
    cv::Rect rect(centerRect_.x() - labelX_, centerRect_.y() - labelY_, centerRect_.width(), centerRect_.height());
    image_crop_cv(rect);
    emit Image_crop::send_crop_mat(Output);
}

void Image_crop::image_crop_cv(cv::Rect rect) {
    cv::Rect src_rect(0, 0, Input.cols, Input.rows);
    rect = rect & src_rect;
    Output.create(cv::Size(rect.width, rect.height), Input.type());
    Input(rect).copyTo(Output);
}


void Image_crop::mousePressEvent(QMouseEvent* e) {
    if (centerRect_.contains(e->pos(), true)) {
        start_pos_ = e->pos();
        center_move_location_ = centerRect_;
        press_ = true;
    }
}

void Image_crop::mouseMoveEvent(QMouseEvent* e) {
    if (press_) {
        QPoint move_ = e->pos() - start_pos_;
        /*
        * 限制裁剪框在label内
        * label_x <= end_pos_.x <= label_x + width - centerRect.width
        * label_y <= end_pos_.y <= label_y + heigth - centerRect.heigth
        *
        */
        centerRect_.setRect(std::max(std::min(center_move_location_.x() + move_.x(), labelX_ + labelWidth_ - center_move_location_.width() - sLineWidth_), labelX_),
            std::max(std::min(center_move_location_.y() + move_.y(), labelY_ + labelHeigth_ - center_move_location_.height() - sLineWidth_), labelY_),
            centerRect_.width(), centerRect_.height());
        this->update();
    }
}

void Image_crop::mouseReleaseEvent(QMouseEvent* e)
{
    press_ = false;
}

bool Image_crop::eventFilter(QObject* obj, QEvent* e) {


    if (obj == labelLeftBottom_
        || obj == labelRightBottom_
        || obj == labelLeftTop_
        || obj == labelRightTop_) {
        if (e->type() == QEvent::MouseButtonPress) {
            QMouseEvent* event = (QMouseEvent*)e;
            /*
            * LeftButton 表示鼠标左键按下
            * NoButton 表示鼠标在移动
            */
            if (event->buttons() & Qt::LeftButton || event->buttons() == Qt::NoButton) {
                button_press = true;
                global_start_pos_ = event->globalPos();
                dragZoomLocation_ = centerRect_;
                return true;
            }
        }
        else if (e->type() & QEvent::MouseMove) {
            QMouseEvent* event = (QMouseEvent*)e;
            if (button_press && event->buttons() & Qt::LeftButton) {
                QRect rc = dragZoomLocation_;
                int dx = event->globalPos().x() - global_start_pos_.x();
                int dy = event->globalPos().y() - global_start_pos_.y();
                if (obj == labelLeftBottom_) {
                    if (fix_crop) {
                        /*
                        * 如果 dx > dy 以dx为主 按照比例值改变dy
                        */
                        if (std::abs(dx) > std::abs(dy)) {
                            centerRect_.setLeft(std::max(rc.left() + dx, labelX_));
                            if (centerRect_.left() + sCenterRectMinWidth_ > centerRect_.right()) {
                                centerRect_.setLeft(centerRect_.right() - sCenterRectMinWidth_);
                            }
                            centerRect_.setBottom(rc.top() + cvRound(centerRect_.width() / fixCenterRectRatio_));
                            if (centerRect_.bottom() > labelY_ + labelHeigth_ - sLineWidth_) {
                                centerRect_.setBottom(labelY_ + labelHeigth_ - sLineWidth_);
                                centerRect_.setLeft(rc.right() - cvRound(centerRect_.height() * fixCenterRectRatio_));
                            }
                        }
                        else {
                            centerRect_.setBottom(std::min(rc.bottom() + dy, labelY_ + labelHeigth_ - sLineWidth_));
                            if (centerRect_.bottom() - sCenterRectMinWidth_ < centerRect_.top()) {
                                centerRect_.setBottom(rc.top() + sCenterRectMinWidth_);
                            }
                            centerRect_.setLeft(rc.right() - cvRound(centerRect_.height() * fixCenterRectRatio_));
                            if (centerRect_.left() < labelX_) {
                                centerRect_.setLeft(labelX_);
                                centerRect_.setBottom(rc.top() + cvRound(centerRect_.width() / fixCenterRectRatio_));
                            }
                        }
                    }
                    else {
                        centerRect_.setLeft(rc.left() + dx);
                        centerRect_.setBottom(rc.bottom() + dy);
                        if (centerRect_.left() + sCenterRectMinWidth_ > centerRect_.right()) {
                            centerRect_.setLeft(centerRect_.right() - sCenterRectMinWidth_);
                        }
                        if (centerRect_.bottom() - sCenterRectMinWidth_ < centerRect_.top()) {
                            centerRect_.setBottom(centerRect_.top() + sCenterRectMinWidth_);
                        }
                    }
                }
                else if (obj == labelRightBottom_) {
                    if (fix_crop) {
                        if (std::abs(dx) > std::abs(dy)) {
                            centerRect_.setRight(std::min(rc.right() + dx, labelX_ + labelWidth_ - sLineWidth_));
                            if (centerRect_.right() - sCenterRectMinWidth_ < centerRect_.left()) {
                                centerRect_.setRight(centerRect_.left() + sCenterRectMinWidth_);
                            }
                            centerRect_.setBottom(rc.top() + cvRound(centerRect_.width() / fixCenterRectRatio_));
                            if (centerRect_.bottom() > labelY_ + labelHeigth_ - sLineWidth_) {
                                centerRect_.setBottom(labelY_ + labelHeigth_ - sLineWidth_);
                                centerRect_.setRight(rc.left() + cvRound(centerRect_.height() * fixCenterRectRatio_));
                            }
                        }
                        else {
                            centerRect_.setBottom(std::min(rc.bottom() + dy, labelY_ + labelHeigth_ - sLineWidth_));
                            if (centerRect_.bottom() - sCenterRectMinWidth_ < centerRect_.top()) {
                                centerRect_.setBottom(rc.top() + sCenterRectMinWidth_);
                            }
                            centerRect_.setRight(rc.left() + cvRound(centerRect_.height() * fixCenterRectRatio_));
                            if (centerRect_.right() > labelX_ + +labelWidth_ - sLineWidth_) {
                                centerRect_.setRight(labelX_ + +labelWidth_ - sLineWidth_);
                                centerRect_.setBottom(rc.top() + cvRound(centerRect_.width() / fixCenterRectRatio_));
                            }
                        }
                    }
                    else {
                        centerRect_.setRight(rc.right() + dx);
                        centerRect_.setBottom(rc.bottom() + dy);
                        if (centerRect_.right() - sCenterRectMinWidth_ < centerRect_.left()) {
                            centerRect_.setRight(centerRect_.left() + sCenterRectMinWidth_);
                        }
                        if (centerRect_.bottom() - sCenterRectMinWidth_ < centerRect_.top()) {
                            centerRect_.setBottom(centerRect_.top() + sCenterRectMinWidth_);
                        }
                    }
                }
                else if (obj == labelLeftTop_) {
                    // labelx < rc.left < rc.right - minwidth
                    // labely < rc.top < rc.bottom - minwidth
                    if (fix_crop) {
                        if (std::abs(dx) > std::abs(dy)) {
                            centerRect_.setLeft(std::max(rc.left() + dx, labelX_));
                            if (centerRect_.left() + sCenterRectMinWidth_ > centerRect_.right()) {
                                centerRect_.setLeft(centerRect_.right() - sCenterRectMinWidth_);
                            }
                            centerRect_.setTop(rc.bottom() - cvRound(centerRect_.width() / fixCenterRectRatio_));
                            if (centerRect_.top() < labelY_) {
                                centerRect_.setTop(labelY_);
                                centerRect_.setLeft(rc.right() - cvRound(centerRect_.height() * fixCenterRectRatio_));
                            }
                        }
                        else {
                            centerRect_.setTop(std::max(rc.top() + dy, labelY_));
                            if (centerRect_.top() + sCenterRectMinWidth_ > centerRect_.bottom()) {
                                centerRect_.setTop(rc.bottom() - sCenterRectMinWidth_);
                            }
                            centerRect_.setLeft(rc.right() - cvRound(centerRect_.height() * fixCenterRectRatio_));
                            if (centerRect_.left() < labelX_) {
                                centerRect_.setLeft(labelX_);
                                centerRect_.setTop(rc.bottom() - cvRound(centerRect_.width() / fixCenterRectRatio_));
                            }
                        }
                    }
                    else {
                        centerRect_.setLeft(rc.left() + dx);
                        centerRect_.setTop(rc.top() + dy);
                        if (centerRect_.left() + sCenterRectMinWidth_ > centerRect_.right()) {
                            centerRect_.setLeft(centerRect_.right() - sCenterRectMinWidth_);
                        }
                        if (centerRect_.top() + sCenterRectMinWidth_ > centerRect_.bottom()) {
                            centerRect_.setTop(centerRect_.bottom() - sCenterRectMinWidth_);
                        }
                    }
                }
                else if (obj == labelRightTop_) {
                    if (fix_crop) {
                        if (std::abs(dx) > std::abs(dy)) {
                            centerRect_.setRight(std::min(rc.right() + dx, labelX_ + labelWidth_ - sLineWidth_));
                            if (centerRect_.right() - sCenterRectMinWidth_ < centerRect_.left()) {
                                centerRect_.setRight(centerRect_.left() + sCenterRectMinWidth_);
                            }
                            centerRect_.setTop(rc.bottom() - cvRound(centerRect_.width() / fixCenterRectRatio_));
                            if (centerRect_.top() < labelY_) {
                                centerRect_.setTop(labelY_);
                                centerRect_.setRight(rc.left() + cvRound(centerRect_.height() * fixCenterRectRatio_));
                            }
                        }
                        else {
                            centerRect_.setTop(std::max(rc.top() + dy, labelY_));
                            if (centerRect_.top() + sCenterRectMinWidth_ > centerRect_.bottom()) {
                                centerRect_.setTop(rc.bottom() - sCenterRectMinWidth_);
                            }
                            centerRect_.setRight(rc.left() + cvRound(centerRect_.height() * fixCenterRectRatio_));
                            if (centerRect_.right() > labelX_ + +labelWidth_ - sLineWidth_) {
                                centerRect_.setRight(labelX_ + +labelWidth_ - sLineWidth_);
                                centerRect_.setTop(rc.bottom() - cvRound(centerRect_.width() / fixCenterRectRatio_));
                            }
                        }
                    }
                    else {
                        centerRect_.setRight(rc.right() + dx);
                        centerRect_.setTop(rc.top() + dy);
                        if (centerRect_.right() - sCenterRectMinWidth_ < centerRect_.left()) {
                            centerRect_.setRight(centerRect_.left() + sCenterRectMinWidth_);
                        }
                        if (centerRect_.top() + sCenterRectMinWidth_ > centerRect_.bottom()) {
                            centerRect_.setTop(centerRect_.bottom() - sCenterRectMinWidth_);
                        }
                    }
                }
                this->update();
                return true;
            }

        }
        /*
        * 鼠标松开时候
        */
        else if (e->type() & QEvent::MouseButtonRelease) {
            button_press = false;
            return true;
        }

    }
    return QWidget::eventFilter(obj, e);
}