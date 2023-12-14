#pragma once

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QSplineSeries>
#include <QtCore/QtMath>

class CurveChartView : public QChartView
{
	Q_OBJECT

public:
	CurveChartView(QWidget* parent = Q_NULLPTR);
	~CurveChartView();

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	bool forceCorrectnessFlag;
	bool dragPointFlag;
	bool mousepress_;
	bool stateTargetPoint;
	QPointF beginPoint;
	QPointF endPoint;
	int pointIndex;
	int seriesIndex;
	int dataCount;
	int minValue;
	int maxValue;

	void updateSeriesData(const QPointF& point);

signals:
	void signalupdatedata(QPointF point);
	void signalMouseRelease(bool);
};

