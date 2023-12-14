#include "CurveChartView.h"

CurveChartView::CurveChartView(QWidget* parent)
	: QChartView(parent)
{
	forceCorrectnessFlag = false;
	dragPointFlag = false;
	stateTargetPoint = false;
	mousepress_ = false;
	seriesIndex = 0;
	pointIndex = 0;
	this->setDragMode(QGraphicsView::RubberBandDrag);
	this->setMouseTracking(true);
}


CurveChartView::~CurveChartView()
{
}

void CurveChartView::mousePressEvent(QMouseEvent* event)
{

	mousepress_ = true;
	beginPoint = event->pos();
	emit signalMouseRelease(mousepress_);
	QChartView::mousePressEvent(event);
}


void CurveChartView::mouseMoveEvent(QMouseEvent* event)
{

	QPoint point = event->pos();
	if (mousepress_)
	{
		this->size();
		updateSeriesData(point);
	}
	
	QChartView::mouseMoveEvent(event);
}


void CurveChartView::mouseReleaseEvent(QMouseEvent* event)
{

	mousepress_ = false;
	emit signalMouseRelease(mousepress_);
	QChartView::mouseReleaseEvent(event);
}


void CurveChartView::updateSeriesData(const QPointF& point)
{
	// detect data
	// force and aft to remove
	// get data
	QPointF target = this->chart()->mapToValue(point);
	// restricted y range
	if (target.y() < 0)
		target.setY(0);
	else if (target.y() > 255)
		target.setY(255);
	// replace current point value
	emit signalupdatedata(target);
}