#ifndef XIMAGEVIEWER_H
#define XIMAGEVIEWER_H
//http://www.tuicool.com/articles/6vEZna
#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPainter>

#include <QPaintEvent>
#include <QDebug>
#include <QLabel>
#include "XImageProcess.h"






class XImageViewer:public QWidget
{

    Q_OBJECT
public:
    XImageViewer();

    void SetImg(QString filepath);
    void SetScale(float m_scale);
    float GetWidthFitScale(int m_width);

private:
    bool isContainPoint(QPoint &p);


    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *event);

private:
    QImage Q_Origne_Img;
    QImage Q_Sacled_Img;

    float scalling;

    bool isInitialised;
    bool isStartingCrop;


    // for show the scaled image
    QLabel *ImageLable;

    XImageProcess OriginImgHough;

    //bool ScaledImgHoughReload;
    //XImageProcess ScaledImgHough;

    XPostion ScaledPos,OriginPos;


    bool OriginImgHoughReload;



    QString SaveFileFolder,ImageFileName,ImagePath;


signals:
    void addPage();
    void reducePage();



};

#endif // XIMAGEVIEWER_H
