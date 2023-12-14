#pragma once

#include <QMainWindow>
#include "ui_Image_text.h"
#include "Image_opencv.h"
#include "qlabel.h"
#include "qpen.h"
#include "qpainter.h"
#include "qevent.h"
#include "qtextedit.h"
#include "qfont.h"

class Image_text : public QMainWindow
{
	Q_OBJECT

public:
	Image_text(QWidget* parent = nullptr);
	~Image_text();

private:
	Ui::Image_textClass ui;

	bool CONSTRUCTED_FUNCTION;

	QFont font;
	QRect Painter_Canvas_;//����
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
	QLabel* labeltop_ = nullptr;//���Դ����ƶ����߿�������Ķ���
	QTextEdit* TextEdit_ = nullptr;
	QPoint start_pos_;//����ƶ���ʼλ��
	QPoint end_pos_;//����ƶ�Ŀ��λ��
	QPoint global_start_pos_;//����ƶ�ȫ����ʼλ��
	QRect center_move_location_;//���Ŀ���ʼλ��

	QPixmap pix;
	QImage _pix;

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
	void image_text_cv(cv::Rect rect);
	void check_centerRect();
	virtual void mousePressEvent(QMouseEvent* e) override;
	virtual void mouseMoveEvent(QMouseEvent* e) override;
	virtual void mouseReleaseEvent(QMouseEvent* e)override;
	virtual void paintEvent(QPaintEvent* event) override;
	virtual bool eventFilter(QObject*, QEvent*) override;
	void setRects();
	void after_image_text();

private slots:
	void send_text_mat();

signals:
	void send_ratio(float);
	void send_text_mat(cv::Mat);


};
