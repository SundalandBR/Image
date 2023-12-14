#include"Image_Rotate.h"


//ͼ����ת
void Image_Rotate(cv::InputArray Input, cv::OutputArray Output, double angle,bool black) {

	cv::Mat src = Input.getMat();
	cv::Point2f center;
	center.x = src.cols / 2.0;
	center.y = src.rows / 2.0;
	//��ת����

	cv::Mat rotate_matrix = cv::getRotationMatrix2D(center, angle, 1.0);

	double angle1 = angle * CV_PI / 180;
	double cos = std::abs(rotate_matrix.at<double>(0, 0));
	double sin = std::abs(rotate_matrix.at<double>(0, 1));
	double new_cols = src.rows * sin + src.cols * cos;
	double new_rows = src.rows * cos + src.cols * sin;

	int c, r;
	/*������תģʽ
	* ��תʱ����С �޺ڱ�
	* ��תʱ����С �кڱ�
	*/
	if (black) {
		rotate_matrix.at<double>(0, 2) += (new_cols - src.cols) * 0.5;
		rotate_matrix.at<double>(1, 2) += (new_rows - src.rows) * 0.5;
		c = cvRound(new_cols);
		r = cvRound(new_rows);
	}
	else {
		double angle2 = fabs(std::atan(new_rows / new_cols));
		double len = src.cols * cos;
		double Y = len / (1 / fabs(tan(angle1)) + 1 / fabs(tan(angle2)));
		double X = Y * 1 / fabs(tan(angle2));
		c = cvRound(new_cols - X * 2);
		r = cvRound(new_rows - Y * 2);
		rotate_matrix.at<double>(0, 2) += cvRound((c - src.cols) * 0.5);
		rotate_matrix.at<double>(1, 2) += cvRound((r - src.rows) * 0.5);
	}
	

	//����仯
	cv::warpAffine(src, Output, rotate_matrix, cv::Size(c, r));

	return;
}