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

	QRect Painter_Canvas_;//����

	QRect leftRect_;//�м��ο����������ɲ�Rect
	QRect topRect_;//�м��ο�����Ķ����ɲ�Rect
	QRect rightRect_;//�м��ο�������Ҳ��ɲ�Rect
	QRect bottomRect_;//�м��ο�����ĵײ��ɲ�Rect
	QRect centerRect_;//�м��ο������Rect

	QColor outsideColor_;//�ⲿ�ɲ����ɫֵ
	QColor insideColor_;//�ڲ��ο��������ɫֵ
	QColor dashColor_;//�м��ο���������߱߿����ɫֵ

	QPen dashPen_;//�м��ο���������߱߿��Pen�������������߳��Ⱥͼ�����ȵȣ�
	QVector<qreal> dashes_;//dashPen_�����߳��Ⱥͼ����������

	QLabel* labelLeftBottom_ = nullptr;//���Դ������ţ��ο��������߱߿����²��Ķ���
	QLabel* labelRightBottom_ = nullptr;//���Դ������ţ��ο��������߱߿����²��Ķ���
	QLabel* labelLeftTop_ = nullptr;//���Դ������ţ��ο��������߱߿����ϲ��Ķ���
	QLabel* labelRightTop_ = nullptr;//���Դ������ţ��ο��������߱߿����ϲ��Ķ���

	QPoint start_pos_;//����ƶ���ʼλ��
	QPoint end_pos_;//����ƶ�Ŀ��λ��
	QPoint global_start_pos_;//����ƶ�ȫ����ʼλ��
	QRect center_move_location_;//���Ŀ���ʼλ��

	QPixmap pix;
	QImage _pix;

	float fix_ratio;//�������ŵı���ֵ 16/9,4/3 
	bool fix_crop = false;//�Ƿ񰴱�������
	bool button_press = false;//�Ƿ������Ķ�����ק
	bool press_ = false;//�Ƿ�����ü�����ק

	int labelX_;//����ͼƬ��label���x����
	int labelY_;//����ͼƬ��label���y����
	int labelWidth_;//����ͼƬ��label��Ŀ��
	int labelHeigth_;//����ͼƬ��label��ĳ���

	static int sDragDotWidth_;      //ѡ�������ϼ������ԷŴ���С�ĵ��С
	static int sCenterRectMinWidth_;//ѡ������С��Ⱥ͸߶ȣ���ֹѡ�������ص�
	static int sLineWidth_;//ѡ�����߿��
	static int sDragDotOffset_;//�ĸ������offsetֵ

	QRect    dragMoveLocation_;//����centerRect_�ƶ�ʱ�ĳ�ʼֵ
	QPoint   dragZoomPos_;//�ĸ����㱻QMouseEvent pressʱ�ĳ�ʼֵ
	bool     dragZoomRunning_;//�ĸ����㱻QMouseEvent pressʱ��
	QRect    dragZoomLocation_;//�ĸ����㱻QMouseEvent pressʱ������centerRect_�ĳ�ʼֵ
	float    fixCenterRectRatio_;//ѡ���̶�����ֵ���ţ������ø�ֵʱ�ǹ̶���������
	//std::function<void(QRect rect)> selectRectChange_;//ѡ���仯ʱ�ص�����



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


