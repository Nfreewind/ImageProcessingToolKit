#include "calcolor.h"

int calcolor(Mat &img)
{
    Mat I, Lab, L, a, b;
    resize(img, I, cv::Size(img.cols>>2, img.rows>>2), 0, 0, CV_INTER_NN);
    cvtColor(I, Lab, CV_RGB2Lab);
    vector<Mat>channel;
    split(Lab, channel);
    L = channel.at(0);
    a = channel.at(1);
    b = channel.at(2);

    unsigned int sum_a = 0,
            sum_b = 0,
            hist_a[256] = {0},
            hist_b[256] = {0};

    int w = I.cols,
            h = I.rows,
            num = w * h;

    for (int i = 0; i < h; ++i)
    {
        uchar *p = Lab.ptr<uchar>(i);
        for (int j = 0; j < w; ++j)
        {
            sum_a += *(++p);
            hist_a[*p]++;
            sum_b += *(++p);
            hist_b[*p]++;
            ++p;
        }
    }
    int msq_a = 0,
            msq_b = 0,
            avg_a = (sum_a / num - 128) * 100,
            avg_b = (sum_b / num - 128) * 100;
    for (int i = 0; i < 256; ++i)
        {
            msq_a += (abs(i*100 - avg_a - 12800) * hist_a[i]) / num;
            msq_b += (abs(i*100 - avg_b - 12800) * hist_b[i]) / num;
        }
    return (abs(avg_a) + abs(avg_b)) * 100 / (abs(msq_a) + abs(msq_b));
}


