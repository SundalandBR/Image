// �����˲�
#include "GuidedFilter.h"

extern int row;
extern int col;



// ����ͼI ����Ϊ<uchar>���˲�ͼp ����Ϊ<uchar>,redius �˲��뾶��reg ���򻯲���
cv::Mat GuideFilter(cv::Mat I,cv::Mat p,int redius,double reg) {

	//�˲����ڴ�С
	int WSIZE = 2 * redius + 1;

	//��������ת��
	//��������ʾ����ɫ����ֵ��[0,1]֮��
	I.convertTo(I, CV_64F, 1.0 / 255.0, 0);
	p.convertTo(p, CV_64F, 1.0 / 255.0, 0);


	//meanI = fmean(I)
	cv::Mat mean_I;
	cv::boxFilter(I, mean_I, -1, cv::Size(WSIZE, WSIZE), cv::Point(-1, -1), true, cv::BORDER_REFLECT);

	//meanp = fmean(p)
	cv::Mat mean_p;
	cv::boxFilter(p, mean_p, -1, cv::Size(WSIZE, WSIZE), cv::Point(-1, -1), true, cv::BORDER_REFLECT);

	//corrI = fmean(I.*I)
	cv::Mat IIMUL = I.mul(I);
	cv::Mat corr_I;
	cv::boxFilter(IIMUL, corr_I, -1, cv::Size(WSIZE, WSIZE), cv::Point(-1, -1), true, cv::BORDER_REFLECT);

	//corrIp = fmean(I.*p)
	cv::Mat IpMUL = I.mul(p);
	cv::Mat corr_Ip;
	cv::boxFilter(IpMUL, corr_Ip, -1, cv::Size(WSIZE, WSIZE), cv::Point(-1, -1), true, cv::BORDER_REFLECT);

	//varI = corrI - meanI .* meanI
	cv::Mat varI,meanII;
	meanII = mean_I.mul(mean_I);

	cv::subtract(corr_I, meanII, varI);

	//covIp = corrIp - meanI .* meanp
	cv::Mat covIp, meanIp;
	meanIp = mean_I.mul(mean_p);
	cv::subtract(corr_Ip, meanIp,covIp);

	// a = covIp ./ (varI + reg)
	cv::Mat a;
	cv::divide(covIp, (varI + reg),a);
	
	
	// b = meanp - a.* meanI
	cv::Mat b,t_meanI_a;
	t_meanI_a = a.mul(mean_I);
	cv::subtract(mean_p, t_meanI_a, b);

	// meana = fmean(a) 
	// meanb = fmean(b)
	cv::Mat mean_a,mean_b;
	cv::boxFilter(a, mean_a, -1, cv::Size(WSIZE, WSIZE), cv::Point(-1, -1), true, cv::BORDER_REFLECT);
	cv::boxFilter(b, mean_b, -1, cv::Size(WSIZE, WSIZE), cv::Point(-1, -1), true, cv::BORDER_REFLECT);

	// q = meana .* I + meanb
	cv::Mat q;
	q = mean_a.mul(I) + mean_b;
	
	//��������ת��
	//q.convertTo(q, CV_8U, 255);

	return q;
}
