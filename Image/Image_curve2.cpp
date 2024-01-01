#include "Image_curve2.h"

extern cv::Mat Input;
static cv::Mat dst;

using namespace std;

Image_curve2::Image_curve2(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Input.copyTo(dst);
	Input.copyTo(_src);
	cv::cvtColor(_src, _src, cv::COLOR_BGR2HLS);
	cv::cvtColor(dst, dst, cv::COLOR_BGR2HLS);

	in_point = false;
	press_ = false;


	light_line = new QLineSeries();
	light_scatter = new QScatterSeries();

	saturation_line = new QLineSeries();
	saturation_scatter = new QScatterSeries();
	chartview = new CurveChartView();
	chart = create();
	chartview->setChart(chart);
	ui.horizontalLayout->addWidget(chartview);
	connect(chartview, SIGNAL(signalupdatedata(QPointF)), this, SLOT(slotTargetPoint(QPointF)));
	connect(chartview, SIGNAL(signalMouseRelease(bool)), this, SLOT(slotMousePress(bool)));
	connect(ui.comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(choose_channel(QString)));
	connect(ui.enter, SIGNAL(clicked()), this, SLOT(on_clicked_enterbutton()));
	connect(ui.exit, SIGNAL(clicked()), this, SLOT(on_clicked_exitbutton()));
	light_line->setUseOpenGL(true);
	saturation_line->setUseOpenGL(true);
}

Image_curve2::~Image_curve2()
{}

QChart* Image_curve2::create() {

	org_data.append(QPointF(0, 0));
	org_data.append(QPointF(84, 84));
	org_data.append(QPointF(168, 168));
	org_data.append(QPointF(255, 255));

	target_data_light = org_data;
	target_data_saturation = org_data;

	target_data = &target_data_light;
	lut = lut_light;

	QChart* chart = new QChart();
	chart->legend()->hide();

	for (int i = 0; i < 256; i++) {
		lut_light[i] = i;
		lut_saturation[i] = i;
		light_line->append(QPointF(i, i));
		saturation_line->append(QPointF(i,i));
	}
	light_scatter->append(org_data);
	saturation_scatter->append(org_data);

	QValueAxis* axisx = new  QValueAxis();
	QValueAxis* axisy = new  QValueAxis();
	axisx->setRange(0, 255); //ÉèÖÃxÖá·¶Î§
	axisy->setRange(0, 255); //ÉèÖÃyÖá·¶Î§
	chart->setAxisX(axisx);
	chart->setAxisY(axisy);

	light_line->setPen(QPen(Qt::black, 2));
	light_scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	light_scatter->setMarkerSize(10);
	light_scatter->setBorderColor(QColor(0, 0, 0));
	light_scatter->setBrush(QBrush(QColor(0, 0, 0)));

	saturation_line->setPen(QPen(Qt::red, 2));
	saturation_scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	saturation_scatter->setMarkerSize(10);
	saturation_scatter->setBorderColor(QColor(255, 0, 0));
	saturation_scatter->setBrush(QBrush(QColor(255, 0, 0)));


	chart->addSeries(light_line);
	chart->addSeries(light_scatter);
	chart->setAnimationOptions(QChart::NoAnimation);
	connect(light_scatter, &QScatterSeries::hovered, this, &Image_curve2::PointHoverd);
	connect(saturation_scatter, &QScatterSeries::hovered, this, &Image_curve2::PointHoverd);

	return chart;
}


void Image_curve2::PointHoverd(const QPointF& point, bool state) {
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

void Image_curve2::slotTargetPoint(QPointF point) {

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

void Image_curve2::slotMousePress(bool press) {
	press_ = press;
}

void Image_curve2::updatedata(QPointF point) {
	QLineSeries* old_lineseries = qobject_cast<QLineSeries*>(chart->series().at(0));
	QScatterSeries* old_scatterseries = qobject_cast<QScatterSeries*>(chart->series().at(1));
	double left = x_index - 1 > 0 ? (*target_data)[x_index - 1].x() : 0.f;
	double right = x_index < (*target_data).size() - 1 ? (*target_data)[x_index + 1].x() : 255.f;
	left += 1;
	right -= 1;
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

void Image_curve2::updateMat() {


	cv::Mat lookuptable(1, 256, CV_8UC3);
	for (int i = 0; i < 256; i++) {
		lookuptable.at<cv::Vec3b>(i)[0] = cv::saturate_cast<uchar>(i);
		lookuptable.at<cv::Vec3b>(i)[1] = cv::saturate_cast<uchar>(*(lut_light + i));
		lookuptable.at<cv::Vec3b>(i)[2] = cv::saturate_cast<uchar>(*(lut_saturation + i));
	}
	cv::LUT(_src, lookuptable, dst);
	cv::cvtColor(dst, dst, cv::COLOR_HLS2BGR);
	emit signalsendmat(dst);

}

void Image_curve2::choose_channel(QString channel) {
	QLineSeries* lineseries = qobject_cast<QLineSeries*>(chart->series().at(0));
	QScatterSeries* scatterseries = qobject_cast<QScatterSeries*>(chart->series().at(1));
	chart->removeSeries(lineseries);
	chart->removeSeries(scatterseries);

	if (channel == "Light") {
		lineseries = light_line;
		scatterseries = light_scatter;
		target_data = &target_data_light;
		lut = lut_light;
	}
	else if (channel == "Saturation") {
		lineseries = saturation_line;
		scatterseries = saturation_scatter;
		target_data = &target_data_saturation;
		lut = lut_saturation;
	}
	chart->addSeries(lineseries);
	chart->addSeries(scatterseries);
	chartview->update();
}

void Image_curve2::on_clicked_enterbutton() {
	this->close();
	emit signalsendmat(dst);
}

void Image_curve2::on_clicked_exitbutton() {

	//cv::destroyAllWindows();
	this->close();
	cv::cvtColor(_src, _src, cv::COLOR_HLS2BGR);
	emit signalsendmat(_src);
}