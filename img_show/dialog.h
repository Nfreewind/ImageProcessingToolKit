#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <qfiledialog.h>
#include <qscreen.h>
#include <QMovie>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "androidfiledialog.h"
#include "imadjust_m.h"
#include "imadjust_s.h"
#include "dcp.h"
#include "guidedfilter.h"
#include "calcolor.h"

using namespace std;
using namespace cv;

extern "C"
{
#include "clahe.h"
#include "clahe1.h"
}

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    void setPixmap(QImage *image);
    explicit Dialog(QWidget *parent = 0);
//    IplImage *img = NULL;
//    IplImage *dst = NULL;
    Mat img;
    Mat dst;
    QString imgname; // 图像路径和文件名
    float val; // 普通增强滑块阈值
    int val2; // 自适增强滑块阈值
    int flag; // 增强方法的标志
    int flag1; // 是否分通道处理标志，默认合通道
    int flag2; // 滑块1是否处理标志
    int flag3; // 滑块2是否处理标志
    int flag4; // 滑块3是否处理标志
    ~Dialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_3_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_checkBox_stateChanged(int arg1);

    void openFileNameReady(QString fileName);

    void on_pushButton_4_clicked();

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_toggled(bool checked);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
