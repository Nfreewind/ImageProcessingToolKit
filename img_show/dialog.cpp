#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QScreen *screen = qApp->primaryScreen();
    QSize screenSize = screen->size();
    this->setAttribute(Qt::WA_AcceptTouchEvents, true);
    this->setFixedSize(screenSize);
    ui->horizontalSlider->setVisible(false);
    ui->horizontalSlider_2->setVisible(false);
    ui->horizontalSlider_3->setVisible(false);
    ui->spinBox->setVisible(false);
    ui->spinBox_2->setVisible(false);
    ui->spinBox_3->setVisible(false);
    ui->spinBox_4->setVisible(false);
    ui->comboBox->addItems(QStringList()<<tr("均衡增强")<<tr("光照不均")<<tr("自适增强")<<tr("去雾增强"));
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->comboBox->setVisible(false);
    ui->checkBox->setVisible(false);
    ui->checkBox_2->setVisible(false);
    ui->checkBox_3->setVisible(false);
    val = 2.0; // 普通增强滑块阈值
    val2 = 1; // 自适增强滑块阈值
    flag = 0; // 增强方法的标志
    flag1 = 0; // 是否分通道处理标志，默认合通道
    flag2 = 0; // 滑块1是否处理标志
    flag3 = 0; // 滑块2是否处理标志
    flag4 = 0; // 滑块3是否处理标志
}

Dialog::~Dialog()
{
    delete ui;
}

//void Dialog::setPixmap(QImage *image)
//{
//    ui->label->setPixmap(QPixmap::fromImage(*image));
//}

// 打开按钮
void Dialog::on_pushButton_clicked()
{
//    QString filename = QFileDialog::getOpenFileName(this, tr("选择图像"), "", tr("Image (*.png *.bmp *.jpg)"));
    ui->checkBox->setChecked(false);
    ui->horizontalSlider->setVisible(false);
    ui->horizontalSlider_2->setVisible(false);
    ui->horizontalSlider_3->setVisible(false);
    ui->spinBox->setVisible(false);
    ui->spinBox_2->setVisible(false);
    ui->spinBox_3->setVisible(false);
    ui->spinBox_4->setVisible(false);
    ui->pushButton_3->setEnabled(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    imgname.clear();
//    QScreen *screen = qApp->primaryScreen();
//    QSize screenSize = screen->size();
//    QFileDialog dlg(this, "Open image");
//    dlg.resize(screenSize.width(), screenSize.height()-200);
//    dlg.setAcceptMode(QFileDialog::AcceptOpen);
//    dlg.setNameFilter("Image (*.png *.bmp *.jpg)");
//    dlg.exec();
//    QStringList files = dlg.selectedFiles();
//    QString filename = files.at(0);
//    if (!( NULL == img))
//        cvReleaseImage(&img);
//    if (!( NULL == dst))
//        cvReleaseImage(&dst);
//    IplImage *src = cvLoadImage((const char *)filename.toLocal8Bit(),1);
    AndroidFileDialog *fileDialog = new AndroidFileDialog();
    connect(fileDialog, SIGNAL(existingFileNameReady(QString)), this, SLOT(openFileNameReady(QString)), Qt::DirectConnection);
    bool success = fileDialog->provideExistingFileName();
    if (!success) {
        disconnect(fileDialog, SIGNAL(existingFileNameReady(QString)), this, SLOT(openFileNameReady(QString)));
    }
    for (bool i = true; i == imgname.isEmpty(); )
    {
        i = true;
    }
//    if(imgname.isEmpty())
//    {
//        ui->label_3->setVisible(true);
//        ui->label_3->setGeometry(200,450,150,313);
//        QMovie *movie = new QMovie(":/new/prefix1/wait.gif");
//        ui->label_3->setMovie(movie);
//        movie->start();
//    }
//    ui->label_3->setVisible(false);
    Mat src = imread((const string)imgname.toLocal8Bit());
    if (src.empty()) return;
    Size imgsize;
    int pixnum = src.rows * src.cols;
    if (pixnum >=14000000)
    {
        imgsize.width = src.cols/5;
        imgsize.height = src.rows/5;
        cv::resize(src, img, imgsize, 0, 0, BORDER_DEFAULT);
    }
    else if ((pixnum >= 7500000)&&(pixnum < 14000000))
    {
        imgsize.width = src.cols/4;
        imgsize.height = src.rows/4;
        cv::resize(src, img, imgsize, 0, 0, BORDER_DEFAULT);
//        img = cvCreateImage(imgsize, src->depth, src->nChannels);
//        cvResize(src, img, BORDER_DEFAULT);
    }
    else if ((pixnum >= 3000000)&&(pixnum < 7500000))
    {
        imgsize.width = src.cols/3;
        imgsize.height = src.rows/3;
        cv::resize(src, img, imgsize, 0, 0, BORDER_DEFAULT);
//        img = cvCreateImage(imgsize, src->depth, src->nChannels);
//        cvResize(src, img, BORDER_DEFAULT);
    }
    else if (pixnum >= 14000000)
    {
        imgsize.width = src.cols/2;
        imgsize.height = src.rows/2;
        cv::resize(src, img, imgsize, 0, 0, BORDER_DEFAULT);
//        img = cvCreateImage(imgsize, src->depth, src->nChannels);
//        cvResize(src, img, BORDER_DEFAULT);
    }
    else img = src;
    if (src.empty()) return;

    // 判断偏色情况
    if (calcolor(img) > 150) flag1 = 2;
    else flag1 = 0;
    cvtColor(img, img, CV_RGB2BGR);
//    cvCvtColor(img, img, CV_RGB2BGR);
    QImage qimg(img.data, img.cols, img.rows, img.step.buf[0], QImage::Format_RGB888);
    QImage imgscale = qimg.scaled(ui->label->size(), Qt::KeepAspectRatio);

//    QImage *img = new QImage;
//    if (!( img->load(filename) ))
//    {
//        delete img;
//        return;
//    }
//    Dialog.setPixmap(img);
    ui->label->setPixmap(QPixmap::fromImage(imgscale));
    ui->label_2->clear();
    ui->pushButton_2->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->checkBox->setVisible(false);
    ui->checkBox_2->setVisible(false);
    ui->checkBox_3->setVisible(true);
    ui->checkBox_3->setEnabled(true);
    if (ui->horizontalSlider->sliderPosition() != 20)
    {
        flag2 = 1;
        ui->horizontalSlider->setSliderPosition(20);
    }
    if (ui->horizontalSlider_2->sliderPosition() != 1)
    {
        flag3 = 1;
        ui->spinBox_2->setValue(1);
        ui->horizontalSlider_2->setSliderPosition(1);
    }
    if (ui->horizontalSlider_3->sliderPosition() != 85)
    {
        flag4 = 1;
        ui->spinBox_4->setValue(85);
        ui->horizontalSlider_3->setSliderPosition(85);
    }
}

// 处理按钮
void Dialog::on_pushButton_2_clicked()
{   
    // CLAHE分通道处理
    if ((flag == 0)&&(flag1 == 2))
    {
        ui->checkBox->stateChanged(2);
        ui->checkBox->setChecked(true);
    }

    // Histeq方法合通道处理
    else if ((flag == 1)&&(flag1 == 0))
    {
        //        dst = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
        dst = Mat::zeros(img.rows, img.cols, img.type());
        unsigned int rgbhist[256] = {0}; // 彩图直方图
        uchar lut[256] = {0}; // 映射表
        int pixnum = img.cols * img.rows;
        int width = img.cols;
        int height = img.rows;
        int i, j;
        for (i = 0; i < height; i++)
        {
            uchar *p = img.ptr<uchar>(i);
            for (j = 0; j < width; j++)
            {
                rgbhist[*p]++;
                rgbhist[*(p+1)]++;
                rgbhist[*(p+2)]++;
                p += 3;
            }
        }
        unsigned int num = 0;
        for (i = 0; i < 256; i++)
        {
            num = num + rgbhist[i];
            lut[i] = (uchar)((float)num / (pixnum * 3) * 255);
        }
        for (i = 0; i < height; i++)
        {
            uchar *p = img.ptr<uchar>(i);
            uchar *pd = dst.ptr<uchar>(i);
            for (j = 0; j < width; j++)
            {
                *pd = lut[*p];
                *(pd+1) = lut[*(p+1)];
                *(pd+2) = lut[*(p+2)];
                p += 3;
                pd += 3;
            }
        }
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->pushButton_3->setEnabled(true);
        ui->checkBox->setVisible(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setVisible(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
        return;
    }

    // Histeq方法分通道处理
    else if ((flag == 1)&&(flag1 == 2))
    {
        ui->checkBox->stateChanged(2);
        ui->checkBox->setChecked(true);
    }

    // CLAHE方法合通道处理
    else if ((flag == 0)&&(flag1 == 0))
    {
        int height = img.rows;
        int width = img.cols;
        vector<Mat>channels;
        vector<Mat>channelsd;
        //        IplImage *r = cvCreateImage(cvGetSize(img), img->depth, 1);
        //        IplImage *g = cvCreateImage(cvGetSize(img), img->depth, 1);
        //        IplImage *b = cvCreateImage(cvGetSize(img), img->depth, 1);
        //        dst = cvCreateImage(cvGetSize(img), img->depth, 3);
        //        cvSplit(img, b, g, r, NULL);
        dst = Mat::zeros(height, width, img.type());
        split(dst, channelsd);
        split(img, channels);
        Mat mtr(channels.at(0));
        Mat mtg(channels.at(1));
        Mat mtb(channels.at(2));

        // 计算余数
        int rowDiv = height % 8 == 0;
        int colDiv = width % 8 == 0;

        if (!(rowDiv && colDiv))
        {
            int rowTileDim = 0;
            int colTileDim = 0;
            int padRow = 0;
            int padCol = 0;
            //            Mat Ipadr, Ipadg, Ipadb;
            // 行
            if (!rowDiv)
            {
                rowTileDim = (int)floor((float)height/8) + 1;
                padRow = rowTileDim * 8 - height;
            }
            else
            {
                rowTileDim = height / 8;
            }

            // 列
            if (!colDiv)
            {
                colTileDim = (int)floor((float)width/8) + 1;
                padCol = colTileDim * 8 - width;
            }
            else
            {
                colTileDim = width / 8;
            }

            // 计算扩边
            if (!(rowTileDim % 2 == 0)) padRow = padRow + 8;
            if (!(colTileDim % 2 == 0)) padCol = padCol + 8;

            // 扩边数
            int padRowPre  = (int)floor((float)padRow/2);
            int padRowPost = (int)ceil((float)padRow/2);
            int padColPre  = (int)floor((float)padCol/2);
            int padColPost = (int)ceil((float)padCol/2);

            // 所有通道一起处理
            copyMakeBorder(mtr, mtr, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
            copyMakeBorder(mtg, mtg, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
            copyMakeBorder(mtb, mtb, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
            CLAHE2(mtr.data, mtg.data, mtb.data, mtr.rows, mtr.cols, 0, 255, 8, 8, 256, 2.0f);
            mtr = mtr(Rect(padColPre,padRowPre,img.cols,img.rows));
            mtg = mtg(Rect(padColPre,padRowPre,img.cols,img.rows));
            mtb = mtb(Rect(padColPre,padRowPre,img.cols,img.rows));
            //            cvReleaseImage(&r);
            //            r = cvCreateImage(cvSize(Ipadr.cols, Ipadr.rows), img->depth, 1);
            //            IplImage r_tmp = IplImage(Ipadr);
            //            r = &r_tmp;
            //            CLAHE1(mtr.data, mtr.rows, mtr.cols, 0, 255, 8, 8, 256, 1.5f);
            //            cvSetImageROI(r, cvRect(padColPre,padRowPre,img->width,img->height));
            // 处理绿色分量
            //            cvReleaseImage(&g);
            //            g = cvCreateImage(cvSize(Ipadg.cols, Ipadg.rows), img->depth, 1);
            //            IplImage g_tmp = IplImage(Ipadg);
            //            g = &g_tmp;
            //            CLAHE1(mtg.data, mtg.rows, mtg.cols, 0, 255, 8, 8, 256, 1.5f);
            //            cvSetImageROI(g, cvRect(padColPre,padRowPre,img->width,img->height));
            // 处理蓝色分量
            //            cvReleaseImage(&b);
            //            b = cvCreateImage(cvSize(Ipadb.cols, Ipadb.rows), img->depth, 1);
            //            IplImage b_tmp = IplImage(Ipadb);
            //            b = &b_tmp;
            //            CLAHE1(mtb.data, mtb.rows, mtb.cols, 0, 255, 8, 8, 256, 1.5f);
            //            cvSetImageROI(b, cvRect(padColPre,padRowPre,img->width,img->height));

            // 将处理后的通道合并
            channelsd.at(0) = mtr;
            channelsd.at(1) = mtg;
            channelsd.at(2) = mtb;
            merge(channelsd, dst);
            //            cvMerge(b, g, r, NULL, dst);
        }
        else
        {
            CLAHE2(mtr.data, mtg.data, mtb.data, height, width, 0, 255, 8, 8, 256, 2.0f);
            channelsd.at(0) = mtr;
            channelsd.at(1) = mtg;
            channelsd.at(2) = mtb;
            merge(channelsd, dst);
            //            cvMerge(b, g, r, NULL, dst);
        }
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->horizontalSlider->setVisible(true);
        ui->spinBox->setVisible(true);
        ui->pushButton_3->setEnabled(true);
        ui->checkBox->setVisible(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setVisible(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
        return;
    }

    // imadjust合通道处理
    else if ((flag == 2)&&(flag1 == 0))
    {
        int height = img.rows;
        int width = img.cols;
        img.copyTo(dst);
        imadjust_m(dst, height, width, 1, 99);
        QImage qimg(dst.data, width, height, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->horizontalSlider_2->setVisible(true);
        ui->pushButton_3->setEnabled(true);
        ui->checkBox->setVisible(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setVisible(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
        if (val2 <= 11)
        {
            ui->spinBox_2->setVisible(true);
            ui->spinBox_3->setVisible(false);
        }
        else
        {
            ui->spinBox_2->setVisible(false);
            ui->spinBox_3->setVisible(true);
        }
        return;
    }

    // imadjust分通道处理
    else if ((flag == 2)&&(flag1 == 2))
    {
        ui->checkBox->stateChanged(2);
        ui->checkBox->setChecked(true);
    }
    // DCP处理
    else if (flag == 3)
    {
        int height = img.rows;
        int width = img.cols;
        img.copyTo(dst);
        dcp(dst, 240, 0.05f, 0.85f);
        QImage qimg(dst.data, width, height, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->horizontalSlider_3->setVisible(true);
        ui->pushButton_3->setEnabled(true);
        ui->checkBox->setVisible(true);
        ui->checkBox->setEnabled(false);
        ui->checkBox_2->setVisible(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
        ui->spinBox_4->setVisible(true);
    }
}

void Dialog::on_horizontalSlider_2_valueChanged(int value)
{
    if (flag3 == 1)
    {
        flag3 = 0;
        return;
    }
    val2 = value;

    // 合通道
    if (flag1 == 0)
    {
        int height = img.rows;
        int width = img.cols;
        img.copyTo(dst);
        imadjust_m(dst, height, width, val2, 100-val2);
        QImage qimg(dst.data, width, height, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->horizontalSlider_2->setVisible(true);
        ui->pushButton_3->setEnabled(true);
        ui->checkBox->setVisible(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setVisible(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
        if (val2 <= 11)
        {
            ui->spinBox_2->setVisible(true);
            ui->spinBox_3->setVisible(false);
        }
        else
        {
            ui->spinBox_2->setVisible(false);
            ui->spinBox_3->setVisible(true);
        }
        return;
    }

    // 分通道
    else if (flag1 == 2)
    {
        int height = img.rows;
        int width = img.cols;
        img.copyTo(dst);
        imadjust_s(dst, height, width, val2, 100-val2);
        QImage qimg(dst.data, width, height, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->horizontalSlider_2->setVisible(true);
        ui->pushButton_3->setEnabled(true);
        ui->checkBox->setVisible(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setVisible(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
        if (val2 <= 11)
        {
            ui->spinBox_2->setVisible(true);
            ui->spinBox_3->setVisible(false);
        }
        else
        {
            ui->spinBox_2->setVisible(false);
            ui->spinBox_3->setVisible(true);
        }
    }
}

void Dialog::on_horizontalSlider_valueChanged(int value)
{
    if (flag2 == 1)
    {
        flag2 = 0;
        return;
    }
    val = (float)value / 10;
    if (flag1 == 2)
    {
        int height = img.rows;
        int width = img.cols;
    //    IplImage *r = cvCreateImage(cvGetSize(img), img->depth, 1);
    //    IplImage *g = cvCreateImage(cvGetSize(img), img->depth, 1);
    //    IplImage *b = cvCreateImage(cvGetSize(img), img->depth, 1);
    //    dst = cvCreateImage(cvGetSize(img), img->depth, 3);
    //    cvSplit(img, b, g, r, NULL);
    //    Mat mtr = r;Mat mtg = g;Mat mtb = b;
        vector<Mat>channels;
        vector<Mat>channelsd;
        split(img, channels);
    //    Mat mtr;Mat mtg;Mat mtb;
        Mat mtr(channels.at(0));
        Mat mtg(channels.at(1));
        Mat mtb(channels.at(2));
        dst = Mat::zeros(height, width, img.type());
        split(dst, channelsd);


        // 计算余数
        int rowDiv = height % 8 == 0;
        int colDiv = width % 8 == 0;

        if (!(rowDiv && colDiv))
        {
            int rowTileDim = 0;
            int colTileDim = 0;
            int padRow = 0;
            int padCol = 0;
    //        Mat Ipadr, Ipadg, Ipadb;
            // 行
            if (!rowDiv)
            {
                rowTileDim = (int)floor((float)height/8) + 1;
                padRow = rowTileDim * 8 - height;
            }
            else
            {
                rowTileDim = height / 8;
            }

            // 列
            if (!colDiv)
            {
                colTileDim = (int)floor((float)width/8) + 1;
                padCol = colTileDim * 8 - width;
            }
            else
            {
                colTileDim = width / 8;
            }

            // 计算扩边
            if (!(rowTileDim % 2 == 0)) padRow = padRow + 8;
            if (!(colTileDim % 2 == 0)) padCol = padCol + 8;

            // 扩边数
            int padRowPre  = (int)floor((float)padRow/2);
            int padRowPost = (int)ceil((float)padRow/2);
            int padColPre  = (int)floor((float)padCol/2);
            int padColPost = (int)ceil((float)padCol/2);

            // 处理红色分量
            copyMakeBorder(mtr, mtr, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
    //        cvReleaseImage(&r);
    //        r = cvCreateImage(cvSize(Ipadr.cols, Ipadr.rows), img->depth, 1);
    //        IplImage r_tmp = IplImage(Ipadr);
    //        r = &r_tmp;
            CLAHE1(mtr.data, mtr.rows, mtr.cols, 0, 255, 8, 8, 256, val);
            mtr = mtr(Rect(padColPre,padRowPre,img.cols,img.rows));
    //        cvSetImageROI(r, cvRect(padColPre,padRowPre,img->width,img->height));

            // 处理绿色分量
            copyMakeBorder(mtg, mtg, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
    //        cvReleaseImage(&g);
    //        g = cvCreateImage(cvSize(Ipadg.cols, Ipadg.rows), img->depth, 1);
    //        IplImage g_tmp = IplImage(Ipadg);
    //        g = &g_tmp;
            CLAHE1(mtg.data, mtg.rows, mtg.cols, 0, 255, 8, 8, 256, val);
            mtg = mtg(Rect(padColPre,padRowPre,img.cols,img.rows));
    //        cvSetImageROI(g, cvRect(padColPre,padRowPre,img->width,img->height));

            // 处理蓝色分量
            copyMakeBorder(mtb, mtb, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
    //        cvReleaseImage(&b);
    //        b = cvCreateImage(cvSize(Ipadb.cols, Ipadb.rows), img->depth, 1);
    //        IplImage b_tmp = IplImage(Ipadb);
    //        b = &b_tmp;
            CLAHE1(mtb.data, mtb.rows, mtb.cols, 0, 255, 8, 8, 256, val);
            mtb = mtb(Rect(padColPre,padRowPre,img.cols,img.rows));
    //        cvSetImageROI(b, cvRect(padColPre,padRowPre,img->width,img->height));

            // 将处理后的通道合并
            channelsd.at(0) = mtr;
            channelsd.at(1) = mtg;
            channelsd.at(2) = mtb;
            merge(channelsd, dst);
    //        cvMerge(b, g, r, NULL, dst);
        }
        else
        {
            CLAHE1(mtr.data, height, width, 0, 255, 8, 8, 256, val);
            CLAHE1(mtg.data, height, width, 0, 255, 8, 8, 256, val);
            CLAHE1(mtb.data, height, width, 0, 255, 8, 8, 256, val);
            channelsd.at(0) = mtr;
            channelsd.at(1) = mtg;
            channelsd.at(2) = mtb;
            merge(channelsd, dst);
    //        cvMerge(b, g, r, NULL, dst);
        }
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        return;
    //    cvReleaseImage(&dst);
    }
    else
    {
        int height = img.rows;
        int width = img.cols;
        vector<Mat>channels;
        vector<Mat>channelsd;
//        IplImage *r = cvCreateImage(cvGetSize(img), img->depth, 1);
//        IplImage *g = cvCreateImage(cvGetSize(img), img->depth, 1);
//        IplImage *b = cvCreateImage(cvGetSize(img), img->depth, 1);
//        dst = cvCreateImage(cvGetSize(img), img->depth, 3);
//        cvSplit(img, b, g, r, NULL);
        dst = Mat::zeros(height, width, img.type());
        split(dst, channelsd);
        split(img, channels);
        Mat mtr(channels.at(0));
        Mat mtg(channels.at(1));
        Mat mtb(channels.at(2));

        // 计算余数
        int rowDiv = height % 8 == 0;
        int colDiv = width % 8 == 0;

        if (!(rowDiv && colDiv))
        {
            int rowTileDim = 0;
            int colTileDim = 0;
            int padRow = 0;
            int padCol = 0;
//            Mat Ipadr, Ipadg, Ipadb;
            // 行
            if (!rowDiv)
            {
                rowTileDim = (int)floor((float)height/8) + 1;
                padRow = rowTileDim * 8 - height;
            }
            else
            {
                rowTileDim = height / 8;
            }

            // 列
            if (!colDiv)
            {
                colTileDim = (int)floor((float)width/8) + 1;
                padCol = colTileDim * 8 - width;
            }
            else
            {
                colTileDim = width / 8;
            }

            // 计算扩边
            if (!(rowTileDim % 2 == 0)) padRow = padRow + 8;
            if (!(colTileDim % 2 == 0)) padCol = padCol + 8;

            // 扩边数
            int padRowPre  = (int)floor((float)padRow/2);
            int padRowPost = (int)ceil((float)padRow/2);
            int padColPre  = (int)floor((float)padCol/2);
            int padColPost = (int)ceil((float)padCol/2);

            // 所有通道一起处理
            copyMakeBorder(mtr, mtr, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
            copyMakeBorder(mtg, mtg, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
            copyMakeBorder(mtb, mtb, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
            CLAHE2(mtr.data, mtg.data, mtb.data, mtr.rows, mtr.cols, 0, 255, 8, 8, 256, val);
            mtr = mtr(Rect(padColPre,padRowPre,img.cols,img.rows));
            mtg = mtg(Rect(padColPre,padRowPre,img.cols,img.rows));
            mtb = mtb(Rect(padColPre,padRowPre,img.cols,img.rows));
//            cvReleaseImage(&r);
//            r = cvCreateImage(cvSize(Ipadr.cols, Ipadr.rows), img->depth, 1);
//            IplImage r_tmp = IplImage(Ipadr);
//            r = &r_tmp;
//            CLAHE1(mtr.data, mtr.rows, mtr.cols, 0, 255, 8, 8, 256, 1.5f);
//            cvSetImageROI(r, cvRect(padColPre,padRowPre,img->width,img->height));
            // 处理绿色分量
//            cvReleaseImage(&g);
//            g = cvCreateImage(cvSize(Ipadg.cols, Ipadg.rows), img->depth, 1);
//            IplImage g_tmp = IplImage(Ipadg);
//            g = &g_tmp;
//            CLAHE1(mtg.data, mtg.rows, mtg.cols, 0, 255, 8, 8, 256, 1.5f);
//            cvSetImageROI(g, cvRect(padColPre,padRowPre,img->width,img->height));
            // 处理蓝色分量
//            cvReleaseImage(&b);
//            b = cvCreateImage(cvSize(Ipadb.cols, Ipadb.rows), img->depth, 1);
//            IplImage b_tmp = IplImage(Ipadb);
//            b = &b_tmp;
//            CLAHE1(mtb.data, mtb.rows, mtb.cols, 0, 255, 8, 8, 256, 1.5f);
//            cvSetImageROI(b, cvRect(padColPre,padRowPre,img->width,img->height));

            // 将处理后的通道合并
            channelsd.at(0) = mtr;
            channelsd.at(1) = mtg;
            channelsd.at(2) = mtb;
            merge(channelsd, dst);
//            cvMerge(b, g, r, NULL, dst);
        }
        else
        {
            CLAHE2(mtr.data, mtg.data, mtb.data, height, width, 0, 255, 8, 8, 256, val);
            channelsd.at(0) = mtr;
            channelsd.at(1) = mtg;
            channelsd.at(2) = mtb;
            merge(channelsd, dst);
//            cvMerge(b, g, r, NULL, dst);
        }
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->horizontalSlider->setVisible(true);
        ui->spinBox->setVisible(true);
        ui->pushButton_3->setEnabled(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
        return;
    }
}

void Dialog::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save image"), "/未命名.jpg",
                                                    tr("Images (*.jpg *.png *.bmp)"));
    if (filename.isEmpty()) return;
    cvtColor(dst, dst, CV_RGB2BGR);
    imwrite((const string)filename.toLocal8Bit(), dst);
    return;
//    cvCvtColor(dst, dst, CV_RGB2BGR);
//    cvSaveImage((const char *)filename.toLocal8Bit(), dst, 0);
}

void Dialog::on_comboBox_currentIndexChanged(int index)
{
    flag = index;
    switch (flag)
    {
    case 0:
        ui->horizontalSlider_2->setVisible(false);
        ui->horizontalSlider_3->setVisible(false);
        ui->spinBox_2->setVisible(false);
        ui->spinBox_3->setVisible(false);
        ui->spinBox_4->setVisible(false);
        break;
    case 1:
        ui->horizontalSlider->setVisible(false);
        ui->horizontalSlider_2->setVisible(false);
        ui->horizontalSlider_3->setVisible(false);
        ui->spinBox->setVisible(false);
        ui->spinBox_2->setVisible(false);
        ui->spinBox_3->setVisible(false);
        ui->spinBox_4->setVisible(false);
//        ui->pushButton_3->setEnabled(false);
        break;
    case 2:
        ui->horizontalSlider->setVisible(false);
        ui->horizontalSlider_3->setVisible(false);
        ui->spinBox->setVisible(false);
        ui->spinBox_4->setVisible(false);
        break;
    case 3:
        ui->horizontalSlider->setVisible(false);
        ui->horizontalSlider_2->setVisible(false);
        ui->spinBox->setVisible(false);
        ui->spinBox_2->setVisible(false);
        ui->spinBox_3->setVisible(false);
        ui->checkBox->setEnabled(false);
        break;
    default:
        break;
    }
}

void Dialog::on_checkBox_stateChanged(int arg1)
{
    flag1 = arg1;
//    if (flag == 0) return;

    // CLAHE方法分通道处理
    if ((flag == 0)&&(flag1 == 2))
    {
        int height = img.rows;
        int width = img.cols;
    //    IplImage *r = cvCreateImage(cvGetSize(img), img->depth, 1);
    //    IplImage *g = cvCreateImage(cvGetSize(img), img->depth, 1);
    //    IplImage *b = cvCreateImage(cvGetSize(img), img->depth, 1);
    //    dst = cvCreateImage(cvGetSize(img), img->depth, 3);
    //    cvSplit(img, b, g, r, NULL);
    //    Mat mtr = r;Mat mtg = g;Mat mtb = b;
        vector<Mat>channels;
        vector<Mat>channelsd;
        split(img, channels);
    //    Mat mtr;Mat mtg;Mat mtb;
        Mat mtr(channels.at(0));
        Mat mtg(channels.at(1));
        Mat mtb(channels.at(2));
        dst = Mat::zeros(height, width, img.type());
        split(dst, channelsd);


        // 计算余数
        int rowDiv = height % 8 == 0;
        int colDiv = width % 8 == 0;

        if (!(rowDiv && colDiv))
        {
            int rowTileDim = 0;
            int colTileDim = 0;
            int padRow = 0;
            int padCol = 0;
    //        Mat Ipadr, Ipadg, Ipadb;
            // 行
            if (!rowDiv)
            {
                rowTileDim = (int)floor((float)height/8) + 1;
                padRow = rowTileDim * 8 - height;
            }
            else
            {
                rowTileDim = height / 8;
            }

            // 列
            if (!colDiv)
            {
                colTileDim = (int)floor((float)width/8) + 1;
                padCol = colTileDim * 8 - width;
            }
            else
            {
                colTileDim = width / 8;
            }

            // 计算扩边
            if (!(rowTileDim % 2 == 0)) padRow = padRow + 8;
            if (!(colTileDim % 2 == 0)) padCol = padCol + 8;

            // 扩边数
            int padRowPre  = (int)floor((float)padRow/2);
            int padRowPost = (int)ceil((float)padRow/2);
            int padColPre  = (int)floor((float)padCol/2);
            int padColPost = (int)ceil((float)padCol/2);

            // 处理红色分量
            copyMakeBorder(mtr, mtr, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
    //        cvReleaseImage(&r);
    //        r = cvCreateImage(cvSize(Ipadr.cols, Ipadr.rows), img->depth, 1);
    //        IplImage r_tmp = IplImage(Ipadr);
    //        r = &r_tmp;
            CLAHE1(mtr.data, mtr.rows, mtr.cols, 0, 255, 8, 8, 256, val);
            mtr = mtr(Rect(padColPre,padRowPre,img.cols,img.rows));
    //        cvSetImageROI(r, cvRect(padColPre,padRowPre,img->width,img->height));

            // 处理绿色分量
            copyMakeBorder(mtg, mtg, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
    //        cvReleaseImage(&g);
    //        g = cvCreateImage(cvSize(Ipadg.cols, Ipadg.rows), img->depth, 1);
    //        IplImage g_tmp = IplImage(Ipadg);
    //        g = &g_tmp;
            CLAHE1(mtg.data, mtg.rows, mtg.cols, 0, 255, 8, 8, 256, val);
            mtg = mtg(Rect(padColPre,padRowPre,img.cols,img.rows));
    //        cvSetImageROI(g, cvRect(padColPre,padRowPre,img->width,img->height));

            // 处理蓝色分量
            copyMakeBorder(mtb, mtb, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
    //        cvReleaseImage(&b);
    //        b = cvCreateImage(cvSize(Ipadb.cols, Ipadb.rows), img->depth, 1);
    //        IplImage b_tmp = IplImage(Ipadb);
    //        b = &b_tmp;
            CLAHE1(mtb.data, mtb.rows, mtb.cols, 0, 255, 8, 8, 256, val);
            mtb = mtb(Rect(padColPre,padRowPre,img.cols,img.rows));
    //        cvSetImageROI(b, cvRect(padColPre,padRowPre,img->width,img->height));

            // 将处理后的通道合并
            channelsd.at(0) = mtr;
            channelsd.at(1) = mtg;
            channelsd.at(2) = mtb;
            merge(channelsd, dst);
    //        cvMerge(b, g, r, NULL, dst);
        }
        else
        {
            CLAHE1(mtr.data, height, width, 0, 255, 8, 8, 256, val);
            CLAHE1(mtg.data, height, width, 0, 255, 8, 8, 256, val);
            CLAHE1(mtb.data, height, width, 0, 255, 8, 8, 256, val);
            channelsd.at(0) = mtr;
            channelsd.at(1) = mtg;
            channelsd.at(2) = mtb;
            merge(channelsd, dst);
    //        cvMerge(b, g, r, NULL, dst);
        }
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->horizontalSlider->setVisible(true);
        ui->spinBox->setVisible(true);
        ui->pushButton_3->setEnabled(true);
        ui->checkBox->setVisible(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setVisible(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
        return;
    }

    // Histeq方法合通道处理
    else if ((flag == 1)&&(flag1 == 0))
    {
//        dst = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
        dst = Mat::zeros(img.rows, img.cols, img.type());
        unsigned int rgbhist[256] = {0}; // 彩图直方图
        uchar lut[256] = {0}; // 映射表
        int pixnum = img.cols * img.rows;
        int width = img.cols;
        int height = img.rows;
        int i, j;
        for (i = 0; i < height; i++)
        {
            uchar *p = img.ptr<uchar>(i);
            for (j = 0; j < width; j++)
            {
                rgbhist[*p]++;
                rgbhist[*(p+1)]++;
                rgbhist[*(p+2)]++;
                p += 3;
            }
        }
        unsigned int num = 0;
        for (i = 0; i < 256; i++)
        {
            num = num + rgbhist[i];
            lut[i] = (uchar)((float)num / (pixnum * 3) * 255);
        }
        for (i = 0; i < height; i++)
        {
            uchar *p = img.ptr<uchar>(i);
            uchar *pd = dst.ptr<uchar>(i);
            for (j = 0; j < width; j++)
            {
                *pd = lut[*p];
                *(pd+1) = lut[*(p+1)];
                *(pd+2) = lut[*(p+2)];
                p += 3;
                pd += 3;
            }
        }
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        return;
    }

    // Histeq方法分通道处理
    else if ((flag == 1)&&(flag1 == 2))
    {
//        dst = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
        dst = Mat::zeros(img.rows, img.cols, img.type());
        int pixnum = img.cols * img.rows;
        int width = img.cols;
        int height = img.rows;
        int i, j;
        unsigned int rhist[256] = {0};
        unsigned int ghist[256] = {0};
        unsigned int bhist[256] = {0};
        uchar lutr[256] = {0};
        uchar lutg[256] = {0};
        uchar lutb[256] = {0};
        for (i = 0; i < height; i++)
        {
            uchar *p = img.ptr<uchar>(i);
            for (j = 0; j < width; j++)
            {
                bhist[*p]++;
                ghist[*(p+1)]++;
                rhist[*(p+2)]++;
                p += 3;
            }
        }
        unsigned int numr = 0, numg = 0, numb = 0;
        for (i = 0; i < 256; i++)
        {
            numb = numb + bhist[i];
            numg = numg + ghist[i];
            numr = numr + rhist[i];
            lutb[i] = (uchar)((float)numb / (pixnum) * 255);
            lutg[i] = (uchar)((float)numg / (pixnum) * 255);
            lutr[i] = (uchar)((float)numr / (pixnum) * 255);
        }
        for (i = 0; i < height; i++)
        {
            uchar *p = img.ptr<uchar>(i);
            uchar *pd = dst.ptr<uchar>(i);
            for (j = 0; j < width; j++)
            {
                *pd = lutb[*p];
                *(pd+1) = lutg[*(p+1)];
                *(pd+2) = lutr[*(p+2)];
                p += 3;
                pd += 3;
            }
        }
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->checkBox->setVisible(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setVisible(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
        return;
//        cvReleaseImage(&img1);
    }

    // CLAHE方法合通道处理
    else if ((flag == 0)&&(flag1 == 0))
    {
        int height = img.rows;
        int width = img.cols;
        vector<Mat>channels;
        vector<Mat>channelsd;
//        IplImage *r = cvCreateImage(cvGetSize(img), img->depth, 1);
//        IplImage *g = cvCreateImage(cvGetSize(img), img->depth, 1);
//        IplImage *b = cvCreateImage(cvGetSize(img), img->depth, 1);
//        dst = cvCreateImage(cvGetSize(img), img->depth, 3);
//        cvSplit(img, b, g, r, NULL);
        dst = Mat::zeros(height, width, img.type());
        split(dst, channelsd);
        split(img, channels);
        Mat mtr(channels.at(0));
        Mat mtg(channels.at(1));
        Mat mtb(channels.at(2));

        // 计算余数
        int rowDiv = height % 8 == 0;
        int colDiv = width % 8 == 0;

        if (!(rowDiv && colDiv))
        {
            int rowTileDim = 0;
            int colTileDim = 0;
            int padRow = 0;
            int padCol = 0;
//            Mat Ipadr, Ipadg, Ipadb;
            // 行
            if (!rowDiv)
            {
                rowTileDim = (int)floor((float)height/8) + 1;
                padRow = rowTileDim * 8 - height;
            }
            else
            {
                rowTileDim = height / 8;
            }

            // 列
            if (!colDiv)
            {
                colTileDim = (int)floor((float)width/8) + 1;
                padCol = colTileDim * 8 - width;
            }
            else
            {
                colTileDim = width / 8;
            }

            // 计算扩边
            if (!(rowTileDim % 2 == 0)) padRow = padRow + 8;
            if (!(colTileDim % 2 == 0)) padCol = padCol + 8;

            // 扩边数
            int padRowPre  = (int)floor((float)padRow/2);
            int padRowPost = (int)ceil((float)padRow/2);
            int padColPre  = (int)floor((float)padCol/2);
            int padColPost = (int)ceil((float)padCol/2);

            // 所有通道一起处理
            copyMakeBorder(mtr, mtr, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
            copyMakeBorder(mtg, mtg, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
            copyMakeBorder(mtb, mtb, padRowPre, padRowPost, padColPre, padColPost, BORDER_REFLECT, Scalar() );
            CLAHE2(mtr.data, mtg.data, mtb.data, mtr.rows, mtr.cols, 0, 255, 8, 8, 256, val);
            mtr = mtr(Rect(padColPre,padRowPre,img.cols,img.rows));
            mtg = mtg(Rect(padColPre,padRowPre,img.cols,img.rows));
            mtb = mtb(Rect(padColPre,padRowPre,img.cols,img.rows));
//            cvReleaseImage(&r);
//            r = cvCreateImage(cvSize(Ipadr.cols, Ipadr.rows), img->depth, 1);
//            IplImage r_tmp = IplImage(Ipadr);
//            r = &r_tmp;
//            CLAHE1(mtr.data, mtr.rows, mtr.cols, 0, 255, 8, 8, 256, 1.5f);
//            cvSetImageROI(r, cvRect(padColPre,padRowPre,img->width,img->height));
            // 处理绿色分量
//            cvReleaseImage(&g);
//            g = cvCreateImage(cvSize(Ipadg.cols, Ipadg.rows), img->depth, 1);
//            IplImage g_tmp = IplImage(Ipadg);
//            g = &g_tmp;
//            CLAHE1(mtg.data, mtg.rows, mtg.cols, 0, 255, 8, 8, 256, 1.5f);
//            cvSetImageROI(g, cvRect(padColPre,padRowPre,img->width,img->height));
            // 处理蓝色分量
//            cvReleaseImage(&b);
//            b = cvCreateImage(cvSize(Ipadb.cols, Ipadb.rows), img->depth, 1);
//            IplImage b_tmp = IplImage(Ipadb);
//            b = &b_tmp;
//            CLAHE1(mtb.data, mtb.rows, mtb.cols, 0, 255, 8, 8, 256, 1.5f);
//            cvSetImageROI(b, cvRect(padColPre,padRowPre,img->width,img->height));

            // 将处理后的通道合并
            channelsd.at(0) = mtr;
            channelsd.at(1) = mtg;
            channelsd.at(2) = mtb;
            merge(channelsd, dst);
//            cvMerge(b, g, r, NULL, dst);
        }
        else
        {
            CLAHE2(mtr.data, mtg.data, mtb.data, height, width, 0, 255, 8, 8, 256, val);
            channelsd.at(0) = mtr;
            channelsd.at(1) = mtg;
            channelsd.at(2) = mtb;
            merge(channelsd, dst);
//            cvMerge(b, g, r, NULL, dst);
        }
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->horizontalSlider->setVisible(true);
        ui->spinBox->setVisible(true);
        ui->pushButton_3->setEnabled(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
    }
    // imadjust合通道处理
    else if ((flag == 2)&&(flag1 == 0))
    {
        int height = img.rows;
        int width = img.cols;
        img.copyTo(dst);
        imadjust_m(dst, height, width, val2, 100-val2);
        QImage qimg(dst.data, width, height, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->horizontalSlider->setVisible(false);
        ui->horizontalSlider_2->setVisible(true);
//        ui->spinBox->setVisible(false);
//        ui->horizontalSlider->setVisible(false);
        ui->pushButton_3->setEnabled(true);
        ui->checkBox->setVisible(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setVisible(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
        if (val2 <= 11)
        {
            ui->spinBox_2->setVisible(true);
            ui->spinBox_3->setVisible(false);
        }
        else
        {
            ui->spinBox_2->setVisible(false);
            ui->spinBox_3->setVisible(true);
        }
        return;
    }

    // imadjust分通道处理
    else if ((flag == 2)&&(flag1 == 2))
    {
        int height = img.rows;
        int width = img.cols;
        img.copyTo(dst);
        imadjust_s(dst, height, width, val2, 100-val2);
        QImage qimg(dst.data, width, height, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
        ui->horizontalSlider_2->setVisible(true);
//        ui->spinBox->setVisible(false);
//        ui->horizontalSlider->setVisible(false);
        ui->pushButton_3->setEnabled(true);
        ui->checkBox->setVisible(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setVisible(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(false);
        if (val2 <= 11)
        {
            ui->spinBox_2->setVisible(true);
            ui->spinBox_3->setVisible(false);
        }
        else
        {
            ui->spinBox_2->setVisible(false);
            ui->spinBox_3->setVisible(true);
        }
    }
}

// 去噪选项
void Dialog::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == 2) // 去噪处理
    {
        Mat dstgray;
//        dst.assignTo(dstgray, CV_LOAD_IMAGE_GRAYSCALE);
//        bilateralFilter(dst, dst1, 10, 20, 5);
        cvtColor(dst, dstgray, CV_RGB2GRAY);
        Mat dst1 = guidedFilter(dstgray, dst, 8, 160);
        QImage qimg(dst1.data, dst1.cols, dst1.rows, dst1.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
    }
    else // 不去噪
    {
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
    }
}

void Dialog::openFileNameReady(QString fileName)
{
    if (!fileName.isNull())
    {
        imgname = fileName;
    }
    else
        imgname = "-1";
    return;
}

void Dialog::on_pushButton_4_clicked()
{
    ui->pushButton->setVisible(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton_3->setVisible(true);
    ui->comboBox->setVisible(true);
    ui->pushButton_4->setVisible(false);
    ui->pushButton_4->setEnabled(false);
}

void Dialog::on_horizontalSlider_3_valueChanged(int value)
{
    if (flag4 == 1)
    {
        flag4 = 0;
        return;
    }

    int height = img.rows;
    int width = img.cols;
    img.copyTo(dst);
    dcp(dst, 240, 0.05f, (float)value/100);
    QImage qimg(dst.data, width, height, dst.step.buf[0], QImage::Format_RGB888);
    QImage imgscale = qimg.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
    ui->label_2->setPixmap(QPixmap::fromImage(imgscale));
    ui->horizontalSlider_3->setVisible(true);
    ui->pushButton_3->setEnabled(true);
    ui->checkBox->setVisible(true);
    ui->checkBox->setEnabled(false);
    ui->checkBox_2->setVisible(true);
    ui->checkBox_2->setEnabled(true);
    ui->checkBox_3->setEnabled(false);
    ui->spinBox_4->setVisible(true);
}

void Dialog::on_checkBox_3_toggled(bool checked)
{
    if (checked)
    {
        img.copyTo(dst);
        Mat temp;
        cvtColor(img, temp, CV_RGB2GRAY);
        vector<Mat>channels;
        split(img, channels);
        channels.at(0) = temp;
        channels.at(1) = temp;
        channels.at(2) = temp;
        merge(channels, img);

        QImage qimg(img.data, img.cols, img.rows, img.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label->size(), Qt::KeepAspectRatio);
        ui->label->setPixmap(QPixmap::fromImage(imgscale));
    }
    else
    {
        dst.copyTo(img);

        QImage qimg(img.data, img.cols, img.rows, img.step.buf[0], QImage::Format_RGB888);
        QImage imgscale = qimg.scaled(ui->label->size(), Qt::KeepAspectRatio);
        ui->label->setPixmap(QPixmap::fromImage(imgscale));
    }
}
