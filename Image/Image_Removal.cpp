//去雾算法实现

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


//获取半径为r的窗口内的最小值，用于求取暗通道
static cv::Mat GetDarkChannel(const cv::Mat M, const int redius, double** MinChannel) {

    double** DarkChannel = new double* [row];
    for (int i = 0; i < row; i++) {
        DarkChannel[i] = new double[col];
        std::fill(DarkChannel[i], DarkChannel[i] + col, 1);
    }

    //计算缓冲区大小，缓冲区一次只读入一行或一列
    const int row_buffer = 2 * redius + row;
    const int col_buffer = 2 * redius + col;
    const int buffer_size = max(row_buffer, col_buffer);

    //计算一行或一列分几段
    const int segment_size = 2 * redius + 1;
    const int segment_count = (buffer_size + segment_size - 1) / segment_size;

    //防止缓冲区溢出
    const int max_buffer_size = (segment_count + 1) * segment_size;

    //申请buffer
    double* buffer = new double[max_buffer_size];
    std::fill(buffer, buffer + max_buffer_size, 1);

    //前缀和反向前缀最小值数组
    double* forward = new double[max_buffer_size];
    double* backward = new double[max_buffer_size];
    std::fill(forward, forward + max_buffer_size, 1);
    std::fill(backward, backward + max_buffer_size, 1);

    for (int i = 0; i < row; i++) {

        //将每一行都复制到缓冲区
        std::memcpy(buffer + redius, MinChannel[i], sizeof(double) * col);

        //计算前缀最小值
        for (int s = 0; s < segment_count; s++) {

            //计算每一段的位置
            const int offset = s * segment_size;
            auto for_ptr = forward + offset;
            auto back_ptr = backward + offset;
            auto buf_ptr = buffer + offset;

            //前缀最小和
            *(for_ptr) = *(buf_ptr);
            for (int j = 1; j < segment_size; j++) {
                *(for_ptr + j) = min(*(for_ptr + j - 1), *(buf_ptr + j));
            }

            //反向前缀最小值
            *(back_ptr + segment_size - 1) = *(buf_ptr + segment_size - 1);
            for (int j = segment_size - 2; j >= 0; j--) {
                *(back_ptr + j) = min(*(back_ptr + j + 1), *(buf_ptr + j));
            }
        }

        //计算暗通道
        for (int j = redius; j < redius + col; j++) {

            DarkChannel[i][j - redius] = min(forward[j + redius], backward[j - redius]);

        }

    }

    for (int i = 0; i < col; i++) {

        //将每一列都复制到缓冲区
        for (int k = 0; k < row; k++) {
            std::memcpy(buffer + redius + k, DarkChannel[k] + i, sizeof(double));
        }

        //计算前缀最小值
        for (int s = 0; s < segment_count; s++) {

            //计算每一段的位置
            const int offset = s * segment_size;
            auto for_ptr = forward + offset;
            auto back_ptr = backward + offset;
            auto buf_ptr = buffer + offset;

            //前缀最小和
            *(for_ptr) = *(buf_ptr);
            for (int j = 1; j < segment_size; j++) {
                *(for_ptr + j) = min(*(for_ptr + j - 1), *(buf_ptr + j));
            }

            //反向前缀最小值
            *(back_ptr + segment_size - 1) = *(buf_ptr + segment_size - 1);
            for (int j = segment_size - 2; j >= 0; j--) {
                *(back_ptr + j) = min(*(back_ptr + j + 1), *(buf_ptr + j));
            }
        }

        //在计算完行暗通道的基础上计算列暗通道
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


//获取全局大气光值
static double* Get_atmospheric_light(cv::Mat Input, cv::Mat DarkChannel) {

    // 计算选取像素数量
    int pixel_num = row * col;
    double top_percent = 0.001;
    int light_pixel_num = (int)(pixel_num * top_percent);

    //根据通道数计算每个通道的大气光值
    double* atmospheric_light = new double[Input.channels()]();

    //将暗通道矩阵化为一行矩阵进行排序
    cv::Mat tdc;
    DarkChannel.copyTo(tdc);
    tdc = tdc.reshape(1, 1);
    cv::sort(tdc, tdc, cv::SORT_EVERY_ROW + cv::SORT_DESCENDING);
    double mid = tdc.at<double>(light_pixel_num);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++){
            if (DarkChannel.at<double>(i, j) >= mid) {
                if (Input.channels() > 1) {

                    //BGR通道
                    for (int k = 0; k < Input.channels(); k++) {
                        *(atmospheric_light + k) = max(*(atmospheric_light + k), Input.at<cv::Vec3d>(i,j)[k]);
                    }

                }
                else {

                    //单通道
                    *(atmospheric_light) = max(*(atmospheric_light), Input.at<double>(i, j));

                }
            }
        }
    }

    return atmospheric_light;

}

//获取透射率
static cv::Mat Get_transmission(cv::Mat Input, cv::Mat DarkChannel, double omege, double* atmospheric_light,int redius) {

    // 初始化通道矩阵
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

//复原图像
static cv::Mat Haze_Removal(cv::Mat Input, double* a, cv::Mat t, double t0) {


    //预处理 t
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            double tt = t.at<double>(i, j);
            t.at<double>(i, j) = max(t.at<double>(i, j), t0);
        }
    }

    //去雾图像
    cv::Mat J;

    //多通道情况
    if (Input.channels() > 1) {

        //分离3通道
        std::vector<cv::Mat> p, q;
        cv::split(Input, p);

        for (int i = 0; i < Input.channels(); i++) {

            //J = I-A / max(t,t0) + A
            p[i] = p[i] - *(a + i);
            p[i] /= t;
            p[i] += *(a + i);
            //类型转化
            p[i].convertTo(p[i], CV_8U, 255);
            q.push_back(p[i]);
        }

        //合并3通道
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



//暗通道先验去雾
cv::Mat Imgae_Removal(cv::Mat Input, int redius, double omege, bool GuideFilterFORt, int Guidefilter_redius, double reg, double t0) {
    /*
    * Input 输入图
    * redius 暗通道滤波半径
    * omega 去雾程度
    * GuideFilterFORt 是否对透射率图进行导向滤波
    * Guidefilter_redius 导向滤波半径
    * reg 导向滤波正则化
    * t0 最小雾的传输率
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

    //最小通道图
    double** Minchannel = GetMinChannel(Input);

    //暗通道图
    cv::Mat DarkChannel = GetDarkChannel(Input, redius, Minchannel);
    
    //释放Minchannel内存
    for (int i = 0; i < row; i++) {
        delete[]Minchannel[i];
    }
    delete[]Minchannel;

    //全局大气光值
    double* atmospheric_light = Get_atmospheric_light(Input, DarkChannel);
    for (int i = 0; i < Input.channels(); i++) {
        printf("%lf ", *(atmospheric_light + i));
    }
    printf("\n");

    //透射率图	
    cv::Mat t = Get_transmission(Input, DarkChannel, omege, atmospheric_light,redius);

    t.convertTo(P, CV_8U, 255);
    //cv::imshow("transmission", P);

    //对透射率图做导向滤波
    if (GuideFilterFORt)
    {
        // Get_transmission()函数得到的透射率图是double类型，将其转化为无符号整数类型
        t.convertTo(t, CV_8U, 255);
        t = GuideFilter(I, t, Guidefilter_redius, reg);

        t.convertTo(P, CV_8U, 255);
        //cv::imshow("transmission_G", P);
    }

    //复原图像
    cv::Mat J = Haze_Removal(Input, atmospheric_light, t, t0);
    
    //释放内存
    delete atmospheric_light;
    
    return J;
}



