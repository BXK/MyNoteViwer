#-------------------------------------------------
#
# Project created by QtCreator 2016-10-02T09:19:22
#
#-------------------------------------------------

QT       += core gui

CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyNoteViwer
TEMPLATE = app

INCLUDEPATH += G:\OpenCV\opencv\lib\include
INCLUDEPATH += G:\OpenCV\opencv\lib\include\opencv
INCLUDEPATH += G:\OpenCV\opencv\lib\include\opencv2

release{

LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_calib3d2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_contrib2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_core2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_features2d2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_flann2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_gpu2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_highgui2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_imgproc2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_legacy2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_ml2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_nonfree2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_objdetect2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_ocl2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_photo2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_stitching2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_superres2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_ts2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_video2413.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_videostab2413.lib
}

debug{
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_calib3d2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_contrib2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_core2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_features2d2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_flann2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_gpu2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_highgui2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_imgproc2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_legacy2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_ml2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_nonfree2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_objdetect2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_ocl2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_photo2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_stitching2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_superres2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_ts2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_video2413d.lib
LIBS+=G:\OpenCV\opencv\lib\x64\vc12\lib\opencv_videostab2413d.lib

}



SOURCES += main.cpp\
    xblur.cpp \
    XImageProcess.cpp \
    XImageViewer.cpp \
    MainWindow.cpp

HEADERS  += \
    xblur.h \
    XImageProcess.h \
    XImageViewer.h \
    MainWindow.h

