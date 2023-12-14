//ȥ���㷨ʵ��

#include "Image_Removal.h"

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define MAXPIXEL 255

extern int row, col;
extern cv::Mat Input;

static double** GetMinChannel(cv::Mat M) {


    double** MinChannel = new double* [row];

    for (int i = 0; i < row; i++) {
        MinChannel[i] = new double[col];
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            double MinRGB = 1;
            if (M.channels() > 1) {
                for (int k = 0; k < M.channels(); k++) {
                    MinRGB = min(MinRGB, M.at<cv::Vec3d>(i, j)[k]);
                }
            }
            else {
                MinRGB = min(MinRGB, M.at<double>(i, j));
            }
            MinChannel[i][j] = MinRGB;

        }
    }

    return MinChannel;

}


//��ȡ�뾶Ϊr�Ĵ����ڵ���Сֵ��������ȡ��ͨ��
static cv::Mat GetDarkChannel(const cv::Mat M, const int redius, double** MinChannel) {

    double** DarkChannel = new double* [row];
    for (int i = 0; i < row; i++) {
        DarkChannel[i] = new double[col];
        std::fill(DarkChannel[i], DarkChannel[i] + col, 1);
    }

    //���㻺������С��������һ��ֻ����һ�л�һ��
    const int row_buffer = 2 * redius + row;
    const int col_buffer = 2 * redius + col;
    const int buffer_size = max(row_buffer, col_buffer);

    //����һ�л�һ�зּ���
    const int segment_size = 2 * redius + 1;
    const int segment_count = (buffer_size + segment_size - 1) / segment_size;

    //��ֹ���������
    const int max_buffer_size = (segment_count + 1) * segment_size;

    //����buffer
    double* buffer = new double[max_buffer_size];
    std::fill(buffer, buffer + max_buffer_size, 1);

    //ǰ׺�ͷ���ǰ׺��Сֵ����
    double* forward = new double[max_buffer_size];
    double* backward = new double[max_buffer_size];
    std::fill(forward, forward + max_buffer_size, 1);
    std::fill(backward, backward + max_buffer_size, 1);

    for (int i = 0; i < row; i++) {

        //��ÿһ�ж����Ƶ�������
        std::memcpy(buffer + redius, MinChannel[i], sizeof(double) * col);

        //����ǰ׺��Сֵ
        for (int s = 0; s < segment_count; s++) {

            //����ÿһ�ε�λ��
            const int offset = s * segment_size;
            auto for_ptr = forward + offset;
            auto back_ptr = backward + offset;
            auto buf_ptr = buffer + offset;

            //ǰ׺��С��
            *(for_ptr) = *(buf_ptr);
            for (int j = 1; j < segment_size; j++) {
                *(for_ptr + j) = min(*(for_ptr + j - 1), *(buf_ptr + j));
            }

            //����ǰ׺��Сֵ
            *(back_ptr + segment_size - 1) = *(buf_ptr + segment_size - 1);
            for (int j = segment_size - 2; j >= 0; j--) {
                *(back_ptr + j) = min(*(back_ptr + j + 1), *(buf_ptr + j));
            }
        }

        //���㰵ͨ��
        for (int j = redius; j < redius + col; j++) {

            DarkChannel[i][j - redius] = min(forward[j + redius], backward[j - redius]);

        }

    }

    for (int i = 0; i < col; i++) {

        //��ÿһ�ж����Ƶ�������
        for (int k = 0; k < row; k++) {
            std::memcpy(buffer + redius + k, DarkChannel[k] + i, sizeof(double));
        }

        //����ǰ׺��Сֵ
        for (int s = 0; s < segment_count; s++) {

            //����ÿһ�ε�λ��
            const int offset = s * segment_size;
            auto for_ptr = forward + offset;
            auto back_ptr = backward + offset;
            auto buf_ptr = buffer + offset;

            //ǰ׺��С��
            *(for_ptr) = *(buf_ptr);
            for (int j = 1; j < segment_size; j++) {
                *(for_ptr + j) = min(*(for_ptr + j - 1), *(buf_ptr + j));
            }

            //����ǰ׺��Сֵ
            *(back_ptr + segment_size - 1) = *(buf_ptr + segment_size - 1);
            for (int j = segment_size - 2; j >= 0; j--) {
                *(back_ptr + j) = min(*(back_ptr + j + 1), *(buf_ptr + j));
            }
        }

        //�ڼ������а�ͨ���Ļ����ϼ����а�ͨ��
        for (int j = redius; j < redius + row; j++) {
            DarkChannel[j - redius][i] = min(forward[j + redius], backward[j - redius]);
        }
    }

    cv::Mat DC;
    DC.create(row, col, CV_64F);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            DC.at<double>(i, j) = DarkChannel[i][j];
        }
    }

    return DC;

}


//��ȡȫ�ִ�����ֵ
static double* Get_atmospheric_light(cv::Mat Input, cv::Mat DarkChannel) {

    // ����ѡȡ��������
    int pixel_num = row * col;
    double top_percent = 0.001;
    int light_pixel_num = (int)(pixel_num * top_percent);

    //����ͨ��������ÿ��ͨ���Ĵ�����ֵ
    double* atmospheric_light = new double[Input.channels()]();

    //����ͨ������Ϊһ�о����������
    cv::Mat tdc;
    DarkChannel.copyTo(tdc);
    tdc = tdc.reshape(1, 1);
    cv::sort(tdc, tdc, cv::SORT_EVERY_ROW + cv::SORT_DESCENDING);
    double mid = tdc.at<double>(light_pixel_num);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++){
            if (DarkChannel.at<double>(i, j) >= mid) {
                if (Input.channels() > 1) {

                    //BGRͨ��
                    for (int k = 0; k < Input.channels(); k++) {
                        *(atmospheric_light + k) = max(*(atmospheric_light + k), Input.at<cv::Vec3d>(i,j)[k]);
                    }

                }
                else {

                    //��ͨ��
                    *(atmospheric_light) = max(*(atmospheric_light), Input.at<double>(i, j));

                }
            }
        }
    }

    return atmospheric_light;

}

//��ȡ͸����
static cv::Mat Get_transmission(cv::Mat Input, cv::Mat DarkChannel, double omege, double* atmospheric_light,int redius) {

    // ��ʼ��ͨ������
    double** Transmission_Channel = new double* [row];
    for (int i = 0; i < row; i++) {
        Transmission_Channel[i] = new double[col];
    }

    // tk = 1 - omege * min(min(input / atmospheric_light))
    //min(input / atmospheric_light)
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            double MinBGR = 255;
            for (int k = 0; k < Input.channels(); k++) {
                MinBGR = min(MinBGR, (Input.at<cv::Vec3d>(i, j)[k] / atmospheric_light[k]));
            }
            Transmission_Channel[i][j] = MinBGR;
        }
    }

    cv::Mat transmission;
    transmission = GetDarkChannel(Input, redius, Transmission_Channel);

    // 1 - min(min(input / atmospheric_light))
    cv::Mat t;
    t.create(row, col, CV_64F);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            t.at<double>(i, j) = 1 - (omege * transmission.at<double>(i, j));
        }
    }

    return t;

}

//��ԭͼ��
static cv::Mat Haze_Removal(cv::Mat Input, double* a, cv::Mat t, double t0) {


    //Ԥ���� t
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            double tt = t.at<double>(i, j);
            t.at<double>(i, j) = max(t.at<double>(i, j), t0);
        }
    }

    //ȥ��ͼ��
    cv::Mat J;

    //��ͨ�����
    if (Input.channels() > 1) {

        //����3ͨ��
        std::vector<cv::Mat> p, q;
        cv::split(Input, p);

        for (int i = 0; i < Input.channels(); i++) {

            //J = I-A / max(t,t0) + A
            p[i] = p[i] - *(a + i);
            p[i] /= t;
            p[i] += *(a + i);
            //����ת��
            p[i].convertTo(p[i], CV_8U, 255);
            q.push_back(p[i]);
        }

        //�ϲ�3ͨ��
        cv::merge(q, J);

    }
    else {
        Input.copyTo(J);
        J = J - *a;
        J /= t;
        J += *a;
        J.convertTo(J, CV_8U, 255);
    }


    return J;

}



//��ͨ������ȥ��
cv::Mat Imgae_Removal(cv::Mat Input, int redius, double omege, bool GuideFilterFORt, int Guidefilter_redius, double reg, double t0) {
    /*
    * Input ����ͼ
    * redius ��ͨ���˲��뾶
    * omega ȥ��̶�
    * GuideFilterFORt �Ƿ��͸����ͼ���е����˲�
    * Guidefilter_redius �����˲��뾶
    * reg �����˲�����
    * t0 ��С��Ĵ�����
    */

    cv::Mat I;

    if (Input.channels() > 1) {
        cv::cvtColor(Input, I, cv::COLOR_BGR2GRAY);
        Input.convertTo(Input, CV_64FC3, 1.0 / 255.0);
    }
    else {
        Input.convertTo(Input, CV_64F, 1.0 / 255.0);
    }

    cv::Mat P;

    //��Сͨ��ͼ
    double** Minchannel = GetMinChannel(Input);

    //��ͨ��ͼ
    cv::Mat DarkChannel = GetDarkChannel(Input, redius, Minchannel);
    
    //�ͷ�Minchannel�ڴ�
    for (int i = 0; i < row; i++) {
        delete[]Minchannel[i];
    }
    delete[]Minchannel;

    //ȫ�ִ�����ֵ
    double* atmospheric_light = Get_atmospheric_light(Input, DarkChannel);
    for (int i = 0; i < Input.channels(); i++) {
        printf("%lf ", *(atmospheric_light + i));
    }
    printf("\n");

    //͸����ͼ	
    cv::Mat t = Get_transmission(Input, DarkChannel, omege, atmospheric_light,redius);

    t.convertTo(P, CV_8U, 255);
    //cv::imshow("transmission", P);

    //��͸����ͼ�������˲�
    if (GuideFilterFORt)
    {
        // Get_transmission()�����õ���͸����ͼ��double���ͣ�����ת��Ϊ�޷�����������
        t.convertTo(t, CV_8U, 255);
        t = GuideFilter(I, t, Guidefilter_redius, reg);

        t.convertTo(P, CV_8U, 255);
        //cv::imshow("transmission_G", P);
    }

    //��ԭͼ��
    cv::Mat J = Haze_Removal(Input, atmospheric_light, t, t0);
    
    //�ͷ��ڴ�
    delete atmospheric_light;
    
    return J;
}



