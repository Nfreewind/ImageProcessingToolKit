#include "dcp.h"

void getMin(unsigned char* data,unsigned char* output,int size,int r)
{
    int minIndex=0;
    int block=(r<<1)+1;
    int min=*data;
    for(int i=1;i<block;i++)
    {
        if(min>=*(data+i))
        {
            min=*(data+i);
            minIndex=i;
        }
    }
    output[r]=data[minIndex];
    for (int j=r+1;j<size-r;j++)
    {

        if (minIndex>=j-r)
        {
            int temp=(r<<1)+j-r;
            if(data[minIndex]>=data[temp])
                minIndex=temp;
        }
        else
        {
            minIndex=j-r;
            int min=data[j-r];
            for (int i=j-r+1;i<block+j-r;i++)
            {
                if(min>=data[i])
                {
                    min=data[i];
                    minIndex=i;
                }
            }
        }
        output[j]=data[minIndex];
    }
}

void minFilter2(unsigned char* minData,unsigned char* outputData,int width,int height,int r)
{
    int block=(r<<1)+1;
    unsigned char* tempData=(unsigned char*)calloc(height,sizeof(unsigned char));
    int minIndex=0;
    int min=0;

    // 求行的最小值
    for (int i=0;i<height;i++)
    {
        unsigned char* data_ptr=minData+i*width;
        unsigned char* output_ptr=outputData+i*width;
        getMin(data_ptr,output_ptr,width,r);
    }

    // 求列的最小值
    for (int j=0;j<width;j++)
    {
        min=outputData[j];
        minIndex=0;
        for (int i=1;i<block;i++)
        {
            unsigned char* output_ptr=outputData+i*width;
            if(min>=output_ptr[j])
            {
                min=output_ptr[j];
                minIndex=i;
            }
        }
        *(tempData+r)=min;
        for (int i=r+1;i<height-r;i++)
        {
            if(minIndex>=i-r)
            {
                unsigned char* output_ptr2=outputData+(i+r)*width;
                if(min>=output_ptr2[j])
                {
                    min=output_ptr2[j];
                    minIndex=i+r;
                }
            }
            else
            {
                unsigned char* output_ptr1=outputData+(i-r)*width;
                minIndex=i-r;
                min=output_ptr1[j];
                for (int k=i-r+1;k<block+i-r;k++)
                {
                    unsigned char* output_ptr3=outputData+k*width;
                    if(min>=output_ptr3[j])
                    {
                        min=output_ptr3[j];
                        minIndex=k;
                    }
                }
            }
            *(tempData+i)=min;
        }
        for (int i=r;i<height-r;i++)
        {
            *(outputData+i*width+j)=*(tempData+i);
        }
    }
    free(tempData);
}

void darkchan(Mat &I, Mat &darkimg, int &rsize)
{
    int i, j, row = I.rows, col = I.cols;

    // min(R, G, B)
    for (i = 0; i < row; i++)
    {
        uchar *p1 = I.ptr<uchar>(i);
        uchar *p2 = darkimg.ptr<uchar>(i);
        for (j = 0; j < col; j++)
        {
            *p2 = min(*p1, *(p1+1));
            *p2 = min(*p2, *(p1+2));
            p1 += 3;
            p2 ++;
        }
    }

    // 最小值滤波
    rsize = max(row/100, col/100) >> 1; // 滤波半径大小
    rsize = max(rsize, 3) * 2; // 修正
    copyMakeBorder(darkimg, darkimg, rsize, rsize, rsize, rsize, BORDER_REPLICATE); // 扩边
    Mat dst = Mat(darkimg.rows, darkimg.cols, CV_8UC1);
    minFilter2(darkimg.data, dst.data, darkimg.cols, darkimg.rows, rsize);
    darkimg = dst(Rect(rsize, rsize, col, row));
}

void guidedFilter2(Mat &I, Mat &p, int r, float eps)
{
    /*
    % GUIDEDFILTER   O(1) time implementation of guided filter.
    %
    %   - guidance image: I (should be a gray-scale/single channel image) [ 0, 1 ]
    %   - filtering input image: p (should be a gray-scale/single channel image) [ 0, 1 ]
    %   - local window radius: r
    %   - regularization parameter: eps
    */

    I.convertTo(I, CV_32FC1, 1.0 /255);
    p.convertTo(p, CV_32FC1, 1.0 /255);

    //mean_I = boxfilter(I, r) ./ N;
    Mat mean_I;
    boxFilter(I, mean_I, CV_32FC1, cv::Size(r, r));

    //mean_p = boxfilter(p, r) ./ N;
    Mat mean_p;
    boxFilter(p, mean_p, CV_32FC1, cv::Size(r, r));

    //mean_Ip = boxfilter(I.*p, r) ./ N;
    Mat mean_Ip;
    boxFilter(I.mul(p), mean_Ip, CV_32FC1, cv::Size(r, r));

    //cov_Ip = mean_Ip - mean_I .* mean_p; % this is the covariance of (I, p) in each local patch.
    Mat cov_Ip = mean_Ip - mean_I.mul(mean_p);

    //mean_II = boxfilter(I.*I, r) ./ N;
    Mat mean_II;
    boxFilter(I.mul(I), mean_II, CV_32FC1, cv::Size(r, r));

    //var_I = mean_II - mean_I .* mean_I;
    Mat var_I = mean_II - mean_I.mul(mean_I);

    //a = cov_Ip ./ (var_I + eps); % Eqn. (5) in the paper;
    Mat a = cov_Ip / (var_I + eps);

    //b = mean_p - a .* mean_I; % Eqn. (6) in the paper;
    cv::Mat b = mean_p - a.mul(mean_I);

    //mean_a = boxfilter(a, r) ./ N;
    Mat mean_a;
    boxFilter(a, mean_a, CV_32FC1, cv::Size(r, r));

    //mean_b = boxfilter(b, r) ./ N;
    Mat mean_b;
    boxFilter(b, mean_b, CV_32FC1, cv::Size(r, r));

    //q = mean_a .* I + mean_b; % Eqn. (8) in the paper;
    p = mean_a.mul(I) + mean_b;
}

void dcp(Mat &I, int maxAtomsLight, float eps, float w)
{
    /*
    % DCP   defog algorithm
    %
    %   - source image: I (有雾图像)
    %   - maxAtomsLight: p (最大大气光雾阈值)
    %   - local window radius: w
    %   - regularization parameter: eps
    */
    Mat I_gray, r, g, b, timg;
    vector<Mat>channels;
    int rsize, // 滤波半径
        A, // 大气光雾阈值 atmospheric
        row = I.rows,
        col = I.cols;
    double maxval;
    // 灰度图像
    cvtColor(I, I_gray, CV_BGR2GRAY);
    split(I, channels);
    b = channels.at(0);b.assignTo(b, CV_32FC1);
    g = channels.at(1);g.assignTo(g, CV_32FC1);
    r = channels.at(2);r.assignTo(r, CV_32FC1);

    // 计算按通道图像
    resize(I, I, cv::Size(col >> 2, row >> 2));
    resize(I_gray, I_gray, cv::Size(col >> 2, row >> 2));
    Mat darkimg(I.size(), CV_8UC1);
    darkchan(I, darkimg, rsize);

    // 计算透射率图像
    timg = 255 - w * darkimg;

    // 计算大气光雾值
    minMaxLoc(darkimg, NULL, &maxval, NULL, NULL);
    A = min(maxAtomsLight, (int)maxval);

    // 将灰度图像作为导向图像对透射率图像进行导向滤波
    guidedFilter2(I_gray, timg, rsize * 8 + 1, eps);

    // 滤波后图像的均衡化处理
    resize(timg, timg, cv::Size(col, row));
//    max(timg, 0.3, timg);

    // 恢复无雾图像
    b = (b - (1.0 - timg) * A) / timg;
    b.assignTo(b, CV_8UC1);
    g = (g - (1.0 - timg) * A) / timg;
    g.assignTo(g, CV_8UC1);
    r = (r - (1.0 - timg) * A) / timg;
    r.assignTo(r, CV_8UC1);

    channels.at(0) = b;
    channels.at(1) = g;
    channels.at(2) = r;

    merge(channels, I);
}
