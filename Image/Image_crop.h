#pragma once

#include <QMainWindow>
#include "Image_opencv.h"
#include "ui_Image_crop.h"
#include <qpen.h>
#include <qpainter.h>
#include <qevent.h>


class Image_crop : public QMainWindow
{
	Q_OBJECT

public:
	Image_crop(QWidget *parent = nullptr);
	~Image_crop();

private:
	Ui::Image_cropClass ui;

	bool CONSTRUCTED_FUNCTION;

	QRect Painter_Canvas_;//画布

	QRect leftRect_;//中间镂空区域的左侧蒙层Rect
	QRect topRect_;//中间镂空区域的顶部蒙层Rect
	QRect rightRect_;//中间镂空区域的右侧蒙层Rect
	QRect bottomRect_;//中间镂空区域的底部蒙层Rect
	QRect centerRect_;//中间镂空区域的Rect

	QColor outsideColor_;//外部蒙层的颜色值
	QColor insideColor_;//内部镂空区域的颜色值
	QColor dashColor_;//中间镂空区域的虚线边框的颜色值

	QPen dashPen_;//中间镂空区域的虚线边框的Pen（可以设置虚线长度和间隔长度等）
	QVector<qreal> dashes_;//dashPen_的虚线长度和间隔长度设置

	QLabel* labelLeftBottom_ = nullptr;//可以触发缩放，镂空区域虚线边框左下部的顶点
	QLabel* labelRightBottom_ = nullptr;//可以触发缩放，镂空区域虚线边框右下部的顶点
	QLabel* labelLeftTop_ = nullptr;//可以触发缩放，镂空区域虚线边框左上部的顶点
	QLabel* labelRightTop_ = nullptr;//可以触发缩放，镂空区域虚线边框右上部的顶点

	QPoint start_pos_;//鼠标移动起始位置
	QPoint end_pos_;//鼠标移动目的位置
	QPoint global_start_pos_;//鼠标移动全局起始位置
	QRect center_move_location_;//中心框起始位置

	QPixmap pix;
	QImage _pix;

	float fix_ratio;//比例缩放的比例值 16/9,4/3 
	bool fix_crop = false;//是否按比例缩放
	bool button_press = false;//是否允许四顶点拖拽
	bool press_ = false;//是否允许裁剪框拖拽

	int labelX_;//放置图片的label框的x坐标
	int labelY_;//放置图片的label框的y坐标
	int labelWidth_;//放置图片的label框的宽度
	int labelHeigth_;//放置图片的label框的长度

	static int sDragDotWidth_;      //选择框边线上几个可以放大缩小的点大小
	static int sCenterRectMinWidth_;//选择框的最小宽度和高度，防止选择框边线重叠
	static int sLineWidth_;//选择框边线宽度
	static int sDragDotOffset_;//四个顶点的offset值

	QRect    dragMoveLocation_;//保存centerRect_移动时的初始值
	QPoint   dragZoomPos_;//四个顶点被QMouseEvent press时的初始值
	bool     dragZoomRunning_;//四个顶点被QMouseEvent press时的
	QRect    dragZoomLocation_;//四个顶点被QMouseEvent press时，保存centerRect_的初始值
	float    fixCenterRectRatio_;//选择框固定比例值缩放，不设置该值时非固定比例缩放
	//std::function<void(QRect rect)> selectRectChange_;//选择框变化时回调函数



private:
	void paint_label();
	void image_crop_cv(cv::Rect rect);
	void check_centerRect();
	virtual void mousePressEvent(QMouseEvent* e) override;
	virtual void mouseMoveEvent(QMouseEvent* e) override;
	virtual void mouseReleaseEvent(QMouseEvent* e)override;
	virtual void paintEvent(QPaintEvent* event) override;
	virtual bool eventFilter(QObject*, QEvent*) override;
	void setRects();
	void after_image_crop();

private slots:
	void choose_ratio(QString s);
	void setFixCenterRectRatio(float ratio);
	void send_crop_mat();

signals:
	void send_ratio(float);
	void send_crop_mat(cv::Mat);


};


