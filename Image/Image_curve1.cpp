#include "Image_curve1.h"

extern cv::Mat Input;
static cv::Mat dst;

using namespace std;

Image_curve1::Image_curve1(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Input.copyTo(dst);

	in_point = false;
	press_ = false;


	rgb_line = new QLineSeries();
	rgb_scatter = new QScatterSeries();

	red_line = new QLineSeries();
	red_scatter = new QScatterSeries();

	green_line = new QLineSeries();
	green_scatter = new QScatterSeries();

	blue_line = new QLineSeries();
	blue_scatter = new QScatterSeries();

	chartview = new CurveChartView();
	chart = create();
	chartview->setChart(chart);
	ui.horizontalLayout->addWidget(chartview);

	connect(chartview, SIGNAL(signalupdatedata(QPointF)), this, SLOT(slotTargetPoint(QPointF)));
	connect(chartview, SIGNAL(signalMouseRelease(bool)), this, SLOT(slotMousePress(bool)));
	connect(ui.comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(choose_channel(QString)));
	connect(ui.enter, SIGNAL(clicked()), this, SLOT(on_clicked_enterbutton()));
	connect(ui.exit, SIGNAL(clicked()), this, SLOT(close()));
	rgb_line->setUseOpenGL(true);
	red_line->setUseOpenGL(true);
	green_line->setUseOpenGL(true);
	blue_line->setUseOpenGL(true);

}

Image_curve1::~Image_curve1()
{}

QChart* Image_curve1::create() {

	org_data.append(QPointF(0, 0));
	org_data.append(QPointF(84, 84));
	org_data.append(QPointF(168, 168));
	org_data.append(QPointF(255, 255));

	target_data_rgb = org_data;
	target_data_r = org_data;
	target_data_b = org_data;
	target_data_g = org_data;

	target_data = &target_data_rgb;
	lut = lut_rgb;

	QChart* chart = new QChart();
	chart->legend()->hide();

	vector<pair<int, int>> s;
	for (int i = 0; i < (*target_data).size(); i++) {
		s.emplace_back((*target_data)[i].x(), (*target_data)[i].y());
	}
	spline_point t;
	t = spline(s);
	for (int i = 0; i < 256; i++) {
		lut_rgb[i] = i;
		lut_r[i] = i;
		lut_g[i] = i;
		lut_b[i] = i;
		rgb_line->append(QPointF(i, lut[i]));
		red_line->append(QPointF(i, lut[i]));
		blue_line->append(QPointF(i, lut[i]));
		green_line->append(QPointF(i, lut[i]));

	}
	rgb_scatter->append((*target_data));
	red_scatter->append((*target_data));
	blue_scatter->append((*target_data));
	green_scatter->append((*target_data));

	QValueAxis* axisx = new  QValueAxis();
	QValueAxis* axisy = new  QValueAxis();
	axisx->setRange(0, 255); //…Ë÷√x÷·∑∂Œß
	axisy->setRange(0, 255); //…Ë÷√y÷·∑∂Œß
	chart->setAxisX(axisx);
	chart->setAxisY(axisy);

	rgb_line->setPen(QPen(Qt::black, 2));
	rgb_scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	rgb_scatter->setMarkerSize(10);
	rgb_scatter->setBorderColor(QColor(0, 0, 0));
	rgb_scatter->setBrush(QBrush(QColor(0, 0, 0)));
	red_line->setPen(QPen(Qt::red, 2));
	red_scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	red_scatter->setMarkerSize(10);
	red_scatter->setBorderColor(QColor(255, 0, 0));
	red_scatter->setBrush(QBrush(QColor(255, 0, 0)));
	blue_line->setPen(QPen(Qt::blue, 2));
	blue_scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	blue_scatter->setMarkerSize(10);
	blue_scatter->setBorderColor(QColor(0, 0, 255));
	blue_scatter->setBrush(QBrush(QColor(0, 0, 255)));
	green_line->setPen(QPen(Qt::green, 2));
	green_scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	green_scatter->setMarkerSize(10);
	green_scatter->setBorderColor(QColor(0, 255, 0));
	green_scatter->setBrush(QBrush(QColor(0, 255, 0)));

	chart->addSeries(rgb_line);
	chart->addSeries(rgb_scatter);
	chart->setAnimationOptions(QChart::NoAnimation);
	connect(rgb_scatter, &QScatterSeries::hovered, this, &Image_curve1::PointHoverd);
	connect(red_scatter, &QScatterSeries::hovered, this, &Image_curve1::PointHoverd);
	connect(blue_scatter, &QScatterSeries::hovered, this, &Image_curve1::PointHoverd);
	connect(green_scatter, &QScatterSeries::hovered, this, &Image_curve1::PointHoverd);
	return chart;
}


void Image_curve1::PointHoverd(const QPointF& point, bool state) {
	x_index = 0;
	for (; x_index < (*target_data).size(); x_index++) {
		if (point == (*target_data)[x_index]) {
			break;
		}
	}
	if (state) {
		this->setCursor(Qt::SizeAllCursor);
	}
	else {
		this->setCursor(Qt::ArrowCursor);
	}
	in_point = state;
}

void Image_curve1::slotTargetPoint(QPointF point) {

	qDebug() << in_point << press_;

	if (in_point && press_) {
		if (x_index<0 || x_index >(*target_data).size()) {
			return;
		}
		else {
			updatedata(point);
			chartview->update();
		}
	}
}

void Image_curve1::slotMousePress(bool press) {
	press_ = press;
}

void Image_curve1::updatedata(QPointF point) {
	QLineSeries* old_lineseries = qobject_cast<QLineSeries*>(chart->series().at(0));
	QScatterSeries* old_scatterseries = qobject_cast<QScatterSeries*>(chart->series().at(1));
	double left = x_index - 1 > 0 ? (*target_data)[x_index - 1].x() : 0.f;
	double right = x_index < (*target_data).size() - 1 ? (*target_data)[x_index + 1].x() : 255.f;
	left = (point.x() < left ? left : point.x()) < right ? (point.x() < left ? left : point.x()) : right;
	point.setX((int)left);
	point.setY((int)point.y());
	(*target_data)[x_index] = point;
	vector<pair<int, int>> s;
	for (int i = 0; i < (*target_data).size(); i++) {
		s.emplace_back((*target_data)[i].x(), (*target_data)[i].y());
	}
	spline_point t;
	t = spline(s);
	for (int i = 0; i < 256; i++) {
		lut[i] = spline_Coordinate(i, t);
		old_lineseries->replace(i, QPointF(i, lut[i]));
	}
	old_scatterseries->replace(x_index, point);
	updateMat();
}

void Image_curve1::updateMat() {


	cv::Mat lookuptable(1, 256, CV_8UC3);
	cv::Mat lookuptable_rgb(1, 256, CV_8U);
	for (int i = 0; i < 256; i++) {
		//
		lookuptable_rgb.at<uchar>(i) = cv::saturate_cast<uchar>(*(lut_rgb + i));
		lookuptable.at<cv::Vec3b>(i)[0] = cv::saturate_cast<uchar>(*(lut_b + i));
		lookuptable.at<cv::Vec3b>(i)[1] = cv::saturate_cast<uchar>(*(lut_g + i));
		lookuptable.at<cv::Vec3b>(i)[2] = cv::saturate_cast<uchar>(*(lut_r + i));
	}
	cv::LUT(Input, lookuptable, dst);
	cv::LUT(dst, lookuptable_rgb, dst);
	cv::imshow("‘§¿¿Õº", dst);

}

void Image_curve1::choose_channel(QString channel) {
	QLineSeries* lineseries = qobject_cast<QLineSeries*>(chart->series().at(0));
	QScatterSeries* scatterseries = qobject_cast<QScatterSeries*>(chart->series().at(1));
	chart->removeSeries(lineseries);
	chart->removeSeries(scatterseries);

	if (channel == "RGB") {
		lineseries = rgb_line;
		scatterseries = rgb_scatter;
		target_data = &target_data_rgb;
		lut = lut_rgb;
	}
	else if (channel == "R") {
		lineseries = red_line;
		scatterseries = red_scatter;
		target_data = &target_data_r;
		lut = lut_r;
	}
	else if (channel == "G") {
		lineseries = green_line;
		scatterseries = green_scatter;
		target_data = &target_data_g;
		lut = lut_g;
	}
	else if (channel == "B") {
		lineseries = blue_line;
		scatterseries = blue_scatter;
		target_data = &target_data_b;
		lut = lut_b;
	}
	chart->addSeries(lineseries);
	chart->addSeries(scatterseries);
}

void Image_curve1::on_clicked_enterbutton() {

	//cv::destroyAllWindows();
	this->close();
	emit signalsendmat(dst);
}