#include "Image_text.h"

extern cv::Mat Input;
static cv::Mat Output;

Image_text::Image_text(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    CONSTRUCTED_FUNCTION = true;

    //区域内外等颜色值
    outsideColor_ = QColor(200, 200, 200, 150);
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
    labeltop_ = new QLabel(this);
    labeltop_->setObjectName(QString::fromUtf8("labelTop"));
    labeltop_->setFixedSize(sDragDotWidth_, sDragDotWidth_);
    labeltop_->setCursor(QCursor(Qt::SizeAllCursor));
    labeltop_->setStyleSheet(QString::fromUtf8("background-color: rgb(30,162,255);"));
    TextEdit_ = new QTextEdit(this);
    TextEdit_->setObjectName(QString::fromUtf8("textedit"));
    TextEdit_->setStyleSheet(QString::fromUtf8("background-color: rgba(255,255,255,0);"));

    text_color = Qt::black;
    font = QFont("宋体", 4);
    TextEdit_->setTextColor(text_color);
    TextEdit_->setFont(font);

    labeltop_->installEventFilter(this);
    labelLeftBottom_->installEventFilter(this);
    labelRightBottom_->installEventFilter(this);
    labelLeftTop_->installEventFilter(this);
    labelRightTop_->installEventFilter(this);

    this->update();
    connect(this, SIGNAL(send_ratio(float)), this, SLOT(setFixCenterRectRatio(float)));
    connect(ui.enter_button, SIGNAL(clicked()), this, SLOT(painter_text()));
    connect(ui.exit_button, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui.color_comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(color_combox_change(QString)));
    connect(ui.font_combobox, SIGNAL(currentTextChanged(QString)), this, SLOT(font_combox_change(QString)));
    connect(ui.size_comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(size_combox_change(QString)));
}



Image_text::~Image_text()
{}

int Image_text::sDragDotWidth_ = 9;
int Image_text::sDragDotOffset_ = 3;
int Image_text::sCenterRectMinWidth_ = 6;
int Image_text::sLineWidth_ = 2;


void Image_text::after_image_text() {
    cv::Mat tmat;
    Input.copyTo(tmat);
    cv::cvtColor(tmat, tmat, cv::COLOR_BGR2RGB);
    QImage Qinput = QImage((const unsigned char*)(tmat.data), tmat.cols, tmat.rows, tmat.step, QImage::Format_RGB888);
    ui.text_label->resize(tmat.size().width, tmat.size().height);
    pix = QPixmap::fromImage(Qinput.scaled(ui.text_label->size()));
    labelWidth_ = ui.text_label->width();
    labelHeigth_ = ui.text_label->height();
    labelX_ = ui.text_label->pos().x() - cvRound(labelWidth_ / 2);
    labelY_ = ui.text_label->pos().y() - cvRound(labelHeigth_ / 2);
    centerRect_.setX(labelX_);
    centerRect_.setY(labelY_);
    centerRect_.setWidth(labelWidth_/2);
    centerRect_.setHeight(labelHeigth_/2);
    return;
}

//中心矩阵合法性判断
void Image_text::check_centerRect() {

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

void Image_text::setRects() {
    //edit
    TextEdit_->resize(centerRect_.size());
    TextEdit_->move(QPoint(centerRect_.x(), centerRect_.y()));
}

//update() 调用
void Image_text::paintEvent(QPaintEvent* event)
{

    if (CONSTRUCTED_FUNCTION) {
        CONSTRUCTED_FUNCTION = false;
        after_image_text();
    }

    check_centerRect();

    setRects();

    QPainter painter(this);
    painter.drawPixmap(labelX_, labelY_, labelWidth_, labelHeigth_, pix);
    //move zoomdot
    labelLeftBottom_->move(centerRect_.x() - sDragDotWidth_ / 2 , centerRect_.y() + centerRect_.height() - sDragDotWidth_ / 2 );
    labelRightBottom_->move(centerRect_.x() + centerRect_.width() - sDragDotWidth_ / 2, centerRect_.y() + centerRect_.height() - sDragDotWidth_ / 2 );
    labelLeftTop_->move(centerRect_.x() - sDragDotWidth_ / 2 , centerRect_.y() - sDragDotWidth_ / 2 );
    labelRightTop_->move(centerRect_.x() + centerRect_.width() - sDragDotWidth_ / 2 , centerRect_.y() - sDragDotWidth_ / 2 );
    labeltop_->move(centerRect_.x() + centerRect_.width() / 2, centerRect_.y() - sDragDotWidth_ /2  );
}

void Image_text::mousePressEvent(QMouseEvent* e) {
    if (centerRect_.contains(e->pos(), true)) {
        start_pos_ = e->pos();
        center_move_location_ = centerRect_;
        press_ = true;
    }
}

void Image_text::mouseMoveEvent(QMouseEvent* e) {
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

void Image_text::mouseReleaseEvent(QMouseEvent* e)
{
    press_ = false;
}

bool Image_text::eventFilter(QObject* obj, QEvent* e) {


    if (obj == labelLeftBottom_
        || obj == labelRightBottom_
        || obj == labelLeftTop_
        || obj == labelRightTop_
        || obj == labeltop_) {
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

                    centerRect_.setLeft(rc.left() + dx);
                    centerRect_.setBottom(rc.bottom() + dy);
                    if (centerRect_.left() + sCenterRectMinWidth_ > centerRect_.right()) {
                        centerRect_.setLeft(centerRect_.right() - sCenterRectMinWidth_);
                    }
                    if (centerRect_.bottom() - sCenterRectMinWidth_ < centerRect_.top()) {
                        centerRect_.setBottom(centerRect_.top() + sCenterRectMinWidth_);
                    }

                }
                else if (obj == labelRightBottom_) {

                    centerRect_.setRight(rc.right() + dx);
                    centerRect_.setBottom(rc.bottom() + dy);
                    if (centerRect_.right() - sCenterRectMinWidth_ < centerRect_.left()) {
                        centerRect_.setRight(centerRect_.left() + sCenterRectMinWidth_);
                    }
                    if (centerRect_.bottom() - sCenterRectMinWidth_ < centerRect_.top()) {
                        centerRect_.setBottom(centerRect_.top() + sCenterRectMinWidth_);
                    }

                }
                else if (obj == labelLeftTop_) {
                    // labelx < rc.left < rc.right - minwidth
                    // labely < rc.top < rc.bottom - minwidth
                    centerRect_.setLeft(rc.left() + dx);
                    centerRect_.setTop(rc.top() + dy);
                    if (centerRect_.left() + sCenterRectMinWidth_ > centerRect_.right()) {
                        centerRect_.setLeft(centerRect_.right() - sCenterRectMinWidth_);
                    }
                    if (centerRect_.top() + sCenterRectMinWidth_ > centerRect_.bottom()) {
                        centerRect_.setTop(centerRect_.bottom() - sCenterRectMinWidth_);
                    }

                }
                else if (obj == labelRightTop_) {

                    centerRect_.setRight(rc.right() + dx);
                    centerRect_.setTop(rc.top() + dy);
                    if (centerRect_.right() - sCenterRectMinWidth_ < centerRect_.left()) {
                        centerRect_.setRight(centerRect_.left() + sCenterRectMinWidth_);
                    }
                    if (centerRect_.top() + sCenterRectMinWidth_ > centerRect_.bottom()) {
                        centerRect_.setTop(centerRect_.bottom() - sCenterRectMinWidth_);
                    }

                }
                else if (obj == labeltop_) {
                    centerRect_.setRect(std::max(std::min(rc.x() + dx, labelX_ + labelWidth_ - rc.width() - sLineWidth_), labelX_),
                        std::max(std::min(rc.y() + dy, labelY_ + labelHeigth_ - rc.height() - sLineWidth_), labelY_),
                        centerRect_.width(), centerRect_.height());
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


void Image_text::painter_text() {
    cv::Mat rgbMat;
    cv::cvtColor(Input, rgbMat, cv::COLOR_BGR2RGB);
    QImage _pix(rgbMat.data, rgbMat.cols, rgbMat.rows, static_cast<int>(rgbMat.step), QImage::Format_RGB888);
    QString qstr = this->TextEdit_->toPlainText();
    QPainter pt(&_pix);
    QPen pen = QPen();
    pen.setColor(text_color);
    pt.setPen(pen);
    pt.setFont(font);
    pt.drawText(QRect(centerRect_.x()-labelX_,centerRect_.y()-labelY_,centerRect_.width(),centerRect_.height()), Qt::TextWordWrap | Qt::AlignLeft, qstr);
    Output = cv::Mat(_pix.height(), _pix.width(), CV_8UC3, _pix.bits(), static_cast<size_t>(_pix.bytesPerLine()));
    cv::cvtColor(Output, Output, cv::COLOR_BGR2RGB);
    emit send_text_mat(Output);
    this->close();
}


void Image_text::color_combox_change(QString str) {
    QColor color;
    if (str.compare(QString("黑")) == 0) {
        color = Qt::black;
    }
    else if (str.compare(QString("白")) == 0) {
        color = Qt::white;
    }
    else if (str.compare(QString("红")) == 0) {
        color = Qt::red;
    }
    else if (str.compare(QString("蓝")) == 0) {
        color = Qt::blue;
    }
    else if (str.compare(QString("黄")) == 0) {
        color = Qt::yellow;
    }
    else if (str.compare(QString("绿")) == 0){
        color = Qt::green;
    }
    text_color = color;

    QPalette palette = TextEdit_->palette();

    QTextCharFormat format = TextEdit_->currentCharFormat();

    // 设置文本颜色
    format.setForeground(text_color);

    // 将新的文本格式应用到TextEdit中的所有文本
    TextEdit_->selectAll();
    TextEdit_->mergeCurrentCharFormat(format);

    TextEdit_->update();
}

void Image_text::font_combox_change(QString str) {
    if (str.compare(QString("宋体")) == 0) {
        font.setFamily("宋体");
    }
    else if (str.compare(QString("黑体")) == 0) {
        font.setFamily("黑体");
    }
    else if (str.compare(QString("楷体")) == 0) {
        font.setFamily("楷体");
    }
    else {
        font.setFamily("微软雅黑");
    }
    TextEdit_->setFont(font);
    TextEdit_->update();
}

void Image_text::size_combox_change(QString str) {
    int size = str.toInt();
    font.setPointSize(size);
    TextEdit_->setFont(font);
    TextEdit_->update();
}

void Image_text::change_font() {
    
}