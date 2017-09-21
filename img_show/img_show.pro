#-------------------------------------------------
#
# Project created by QtCreator 2015-05-09T17:45:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += androidextras

TARGET = img_show
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    clahe.c \
    clahe1.c \
    androidfiledialog.cpp \
    imadjust_m.cpp \
    imadjust_s.cpp \
    dcp.cpp \
    guidedfilter.cpp \
    calcolor.cpp

HEADERS  += dialog.h \
    clahe.h \
    clahe1.h \
    androidfiledialog.h \
    imadjust_m.h \
    imadjust_s.h \
    dcp.h \
    guidedfilter.h \
    calcolor.h

FORMS    += dialog.ui

CONFIG += mobility
MOBILITY = 

# Add OpenCV include directory
INCLUDEPATH += D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\jni\include

# Add OpenCV library direcotry and library files
LIBS += D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\3rdparty\libs\armeabi\libIlmImf.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\3rdparty\libs\armeabi\liblibjasper.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\3rdparty\libs\armeabi\liblibjpeg.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\3rdparty\libs\armeabi\liblibpng.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\3rdparty\libs\armeabi\liblibtiff.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_androidcamera.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_calib3d.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_contrib.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_core.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_features2d.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_flann.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_highgui.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_imgproc.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_legacy.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_ml.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_objdetect.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_ocl.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_photo.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_stitching.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_superres.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_ts.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_video.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_videostab.a \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_info.so \
        D:\Software\OpenCV-2.4.9-android-sdk\sdk\native\libs\armeabi\libopencv_java.so

contains(ANDROID_TARGET_ARCH,armeabi) {
    ANDROID_EXTRA_LIBS = \
        D:/Documents/Qt5.3.0/Project/img_show/libopencv_info.so \
        $$PWD/libopencv_java.so
}

RESOURCES += \
    checkbox.qrc
