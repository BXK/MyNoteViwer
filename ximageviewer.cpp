#include "XImageViewer.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include<QImage>
#include <QFileInfo>
#include <QDir>
//#include <QtPlugin>

#include<QImageWriter>
XImageViewer::XImageViewer()
{
    // for key listening
    setFocusPolicy(Qt::StrongFocus);


    isInitialised=false;
    scalling=1;
    isStartingCrop=false;


    ImageLable = new QLabel;
    QVBoxLayout *Image_V_Layout = new QVBoxLayout;
    Image_V_Layout->addStretch(1);
    Image_V_Layout->addWidget(ImageLable);
    Image_V_Layout->addStretch(1);


    QHBoxLayout *Image_H_Layout = new QHBoxLayout;
    Image_H_Layout->addStretch(1);
    Image_H_Layout->addLayout(Image_V_Layout);
    Image_H_Layout->addStretch(1);

    //ScaledImgHoughReload = false;
    OriginImgHoughReload=false;
    this->setLayout(Image_H_Layout);
}




void XImageViewer::SetImg(QString filepath)
{
    ImagePath = filepath;
    QImage tp;
    tp.load(filepath);
    Q_Origne_Img = tp.convertToFormat(QImage::Format_RGB32);

    // Process Original Img
    //OriginImgHough.Process(Q_Origne_Img,filepath,true);
    OriginImgHoughReload=true;


    // scaled Img
    QSize m_size = Q_Origne_Img.size();
    m_size.scale(m_size*scalling,Qt::KeepAspectRatio);
    Q_Sacled_Img = Q_Origne_Img.scaled(m_size,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    // Show Img
    ImageLable->setPixmap(QPixmap::fromImage(Q_Sacled_Img));


    QFileInfo fi = QFileInfo(filepath);
    SaveFileFolder = fi.absolutePath()+"/ImgData";

    ImageFileName = fi.fileName();
    int m_pos = ImageFileName.lastIndexOf(".");
    ImageFileName = ImageFileName.left(m_pos);
}





void XImageViewer::SetScale(float m_scale)
{
    if(!Q_Origne_Img.isNull())
    {
        qDebug()<<m_scale;
        QSize m_size = Q_Origne_Img.size();
        m_size.scale(m_size*m_scale,Qt::KeepAspectRatio);
        Q_Sacled_Img = Q_Origne_Img.scaled(m_size,Qt::KeepAspectRatio,Qt::SmoothTransformation);

        //ScaledImgHoughReload = true;
        scalling = m_scale;

        // Show Img
        ImageLable->setPixmap(QPixmap::fromImage(Q_Sacled_Img));
        update();
    }
}


float XImageViewer::GetWidthFitScale(int m_width)
{
    QSize tp_size = Q_Origne_Img.size();
    float tp_scale = m_width*1.0/tp_size.width();
    //SetScale(tp_scale);
    return tp_scale;
}



bool XImageViewer::isContainPoint(QPoint &p)
{
    if(p.x()>=0 && p.y()>=0 && p.x()<Q_Sacled_Img.size().rwidth() && p.y()<Q_Sacled_Img.size().height())
    {
        return true;
    }
    return false;
}




void XImageViewer::mousePressEvent(QMouseEvent *event)
{
  if ((!Q_Origne_Img.isNull())&&(event->buttons() == Qt::LeftButton))
  {
    if(isContainPoint(ImageLable->mapFromParent(event->pos())))
    {
        qDebug()<<"true";
    }
    else
        qDebug()<<"false";    


    if(OriginImgHoughReload)
    {
        OriginImgHough.Process(Q_Origne_Img,ImagePath,true);
        OriginImgHoughReload =false;
    }

    OriginImgHough.GetBar(ImageLable->mapFromParent(event->pos())/scalling,true,
                          OriginPos.s_bar_top,OriginPos.s_bar_bottom);

    isStartingCrop = true;
  }
}

void XImageViewer::mouseMoveEvent(QMouseEvent *event)
{
  if ((!Q_Origne_Img.isNull())&&(event->buttons() == Qt::LeftButton) && isStartingCrop)
  {

    OriginImgHough.GetBar(ImageLable->mapFromParent(event->pos())/scalling,false,
                          OriginPos.e_bar_top,OriginPos.e_bar_bottom);



    QImage &tp = OriginImgHough.GetSelectedImg(OriginPos);


    QSize m_size = Q_Origne_Img.size();
    m_size.scale(m_size*scalling,Qt::KeepAspectRatio);
    tp = tp.scaled(m_size,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    // Show Img
    ImageLable->setPixmap(QPixmap::fromImage(tp));


    //ImageLable->setPixmap(QPixmap::fromImage(tp));
    update();

//    if (isContainPoint(event->pos()))
//    {
//      cropRect.setEnd(mapToPixmap(event->pos()));
//      update();
//    }
  }
}

void XImageViewer::mouseReleaseEvent(QMouseEvent *e)
{
//  QRect rect(cropRect.startPoint(), cropRect.endPoint());
  isStartingCrop = false;
}


void XImageViewer::keyPressEvent(QKeyEvent *event)
{    


    if(!Q_Origne_Img.isNull())
    {
        if(event->key()==Qt::Key_Q)
        {
            // not save
            ImageLable->setPixmap(QPixmap::fromImage(Q_Sacled_Img));
        }
        else if(event->key()==Qt::Key_Space)
        {
            // save
            QImage tp = OriginImgHough.GetCropedImg(OriginPos);


            QDir xImag;
            int index = 1;
            QString strint=QString("%1").arg(index,3, 10, QChar('0'));
            QString ImageSaveName = SaveFileFolder+"/pic/"+ImageFileName+"_"+strint+".jpg";
            while(xImag.exists(ImageSaveName))
            {
                ++index;
                strint=QString("%1").arg(index,3, 10, QChar('0'));
                ImageSaveName = SaveFileFolder+"/pic/"+ImageFileName+"_"+strint+".jpg";
            }

            QImageWriter writer(ImageSaveName);
            if(!writer.write(tp))
            {
                qDebug() << writer.errorString();
            }

            QFile filetp(SaveFileFolder+"/Log.data");
            if(!filetp.open(QIODevice::Append | QIODevice::Text)) {
                qDebug()<<"Can't open the file!"<<endl;
            }

            QTextStream mout(&filetp);

            mout<<ImageFileName<<","<<
                    OriginPos.s_bar_bottom.x()<<","<<OriginPos.s_bar_bottom.y()<<","<<
                    OriginPos.s_bar_top.x()<<","<<OriginPos.s_bar_top.y()<<","<<
                    OriginPos.e_bar_bottom.x()<<","<<OriginPos.e_bar_bottom.y()<<","<<
                    OriginPos.e_bar_top.x()<<","<<OriginPos.e_bar_top.y()<<endl;
            filetp.close();

            ImageLable->setPixmap(QPixmap::fromImage(Q_Sacled_Img));
        }
        else if(event->key()==Qt::Key_A)
        {
            emit reducePage();

        }
        else if(event->key()==Qt::Key_D)
        {
            emit addPage();
        }
    }
}

