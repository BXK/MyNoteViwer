#include "XImageProcess.h"
#include<vector>
#include <fstream>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include<QGraphicsPixmapItem>
#include<QPainter>
#include<QFileInfo>
#include<QFile>

XImageProcess::XImageProcess()
{

}


void XImageProcess::SaveData(QString filepath)
{
//    int m_MIN;
//    int m_MAX;
//    double mTheta;
//    int num=VHistGram.size();
//    std::vector<int> VHistGram;


    QFile filetp(filepath);
    if(!filetp.open(QIODevice::WriteOnly)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QDataStream mout(&filetp);

    mout<<(qint32)m_MIN<<(qint32)m_MAX<<mTheta<<(qint32)VHistGram.size();
    for(int i=0;i<VHistGram.size();++i)
        mout<<VHistGram[i];
    filetp.close();
}


bool XImageProcess::LoadData(QString filepath)
{
    QFile filetp(filepath);
    if(!filetp.exists())
        return false;

    if(!filetp.open(QIODevice::ReadOnly)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QDataStream mout(&filetp);

    mout>>m_MIN;
    mout>>m_MAX;
    mout>>mTheta;
    qint32 msize_tp;
    mout>>msize_tp;

    VHistGram.assign(msize_tp,0);
    qint32 tp;
    for(int i=0;i<msize_tp;++i)
    {
        mout>>tp;
        VHistGram[i]=tp;
    }
    filetp.close();
    return true;
}

void XImageProcess::Process(QImage qimg,QString filepath,bool flag)
{
    // Origin Img for posprocess
    Q_Origin_Img = qimg.copy();

    // convert to gray Img
    Q_Processed_Img = qimg.copy();
    cv::Mat src = QImage2cvMat(Q_Processed_Img);
    cv::cvtColor(src, C_Processed_Img, CV_RGB2GRAY);
    //cv::bitwise_not(C_Processed_Img,C_Processed_Img);
    cv::GaussianBlur(C_Processed_Img,C_Processed_Img,cv::Size(3,3),0,0);
    cv::Canny(C_Processed_Img,C_Processed_Img,100,200,3);


    if(flag)
    {

        QFileInfo fi = QFileInfo(filepath);

        QString ImageFileName = fi.fileName();
        int m_pos = ImageFileName.lastIndexOf(".");
        ImageFileName = ImageFileName.left(m_pos);

        QString dbpath = fi.absolutePath()+"/ImgData/db/"+ImageFileName+".db";


        if(!LoadData(dbpath))
        {
            // do the hough transformat
            std::vector<cv::Vec2f> lines;
            cv::HoughLines(C_Processed_Img, lines, 1, CV_PI/360, 200);  // 返回直线坐标对
            //float rho = lines[0][0];
            mTheta = lines[0][1];
            Project2VerticalLine(mTheta);
            SaveData(dbpath);
        }
    }
    else
    {
        // do the hough transformat
        std::vector<cv::Vec2f> lines;
        cv::HoughLines(C_Processed_Img, lines, 1, CV_PI/360, 200);  // 返回直线坐标对
        //float rho = lines[0][0];
        mTheta = lines[0][1];
        Project2VerticalLine(mTheta);
    }



    qDebug()<<"mTheta:"<<mTheta;

    std::fstream fout("a.txt");
    for(int i=0;i<VHistGram.size();++i)
    {
        fout<<VHistGram[i]<<std::endl;
    }



    // blur the image;
    Q_Blur_Img = qimg.copy();
    C_Blur_Img = QImage2cvMat(Q_Blur_Img);
    m_Blur.stackblur(C_Blur_Img.data,C_Blur_Img.cols,C_Blur_Img.rows,32);
}







/**
 * @brief XImageProcess::GetBar
 * @param m_point   the input position
 * @param Start_Flag
 * @param bar_top
 * @param bar_bottom
 */
void XImageProcess::GetBar(QPoint m_point,bool Start_Flag,QPoint &bar_top,QPoint &bar_bottom)
{
    // the input point get the projected to theta line
    int index = GetKValue(mTheta,m_point.x(),m_point.y())-m_MIN;

    /************************************ find the line ****************************/
    int m_low = index;
    int m_high= index;

    // this is for bottom and top white line
    /*
     *       m_low1
     * m_low
     *       m_low2
     *      ==================
     *       m_high1
     * m_high
     *       m_high2
     *
     *
     */
    int m_high1=index,m_high2=index;
    int m_low1=index,m_low2=index;

    if(VHistGram.at(index)!=0)
    {
        while(m_high1<VHistGram.size()&&VHistGram.at(m_high1)!=0)
            m_high1++;
        m_high2=m_high1;
        while(m_high2<VHistGram.size()&&VHistGram.at(m_high2)==0)
            m_high2++;
        m_high = (m_high1+m_high2)/2;


        while(m_low2>=0 && VHistGram.at(m_low2)!=0)
            m_low2--;
        m_low1=m_low2;
        while(m_low1>=0 && VHistGram.at(m_low1)==0)
            m_low1--;
        m_low=(m_low1+m_low2)/2;
    }
    else
    {
        // the start of point
        if(Start_Flag)
        {
            while(m_low2<VHistGram.size()&&VHistGram.at(m_low2)==0)
                m_low2++;

            m_low1=index;
            while(m_low1>=0&&VHistGram.at(m_low1)==0)
                m_low1--;
            m_low = (m_low1+m_low2)/2;


            m_high1=m_low2;
            while(m_high1<VHistGram.size() && VHistGram.at(m_high1)!=0)
                m_high1++;

            m_high2=m_high1;
            while(m_high2<VHistGram.size() && VHistGram.at(m_high2)==0)
                m_high2++;
            m_high=(m_high1+m_high2)/2;
        }
        else// the end of point
        {
            while(m_high2<VHistGram.size()&&VHistGram.at(m_high2)==0)
                ++m_high2;

            m_high1=index;
            while(m_high1>=0&&VHistGram.at(m_high1)==0)
                --m_high1;
            m_high = (m_high1+m_high2)/2;


            m_low2=m_high1;
            while(m_low2>=0&& VHistGram.at(m_low2)!=0)
                --m_low2;

            m_low1=m_low2;
            while(m_low1>=0 && VHistGram.at(m_low1)==0)
                m_low1++;
            m_low=(m_low1+m_low2)/2;
        }
    }



/************************************ find the reion of line ****************************/

    /*
    *
    *   y small  bottom
    *   y big   top
    *
    *
    *   bottomLeft  bottomRight
    *
    *
    *   topLeft     topRight
    *
    */

    QPoint topLeft,topRight,bottomLeft,bottomRight;
    // top left (0,y)
    topLeft.setX(0);
    topLeft.setY((m_high+m_MIN)*1.0/sin(mTheta));

    // top right(cols,y)
    topRight.setX(C_Processed_Img.cols);
    topRight.setY(-(cos(mTheta)*topRight.x()-m_high-m_MIN)*1.0/sin(mTheta));

    // bottom left (0,y)
    bottomLeft.setX(0);
    bottomLeft.setY((m_low+m_MIN)*1.0/sin(mTheta));

    // bottom right (cols,y)
    bottomRight.setX(C_Processed_Img.cols);
    bottomRight.setY(-(cos(mTheta)*bottomRight.x()-m_low-m_MIN)*1.0/sin(mTheta));




/******************************* project to horizontal line  *****************************************/


    int tp_MIN = 1;
    int tp_MAX = -1;

    // top left
    int val1 = GetKValue(mTheta-CV_PI/2,topLeft.x(),topLeft.y());
    if(val1>tp_MAX)
        tp_MAX=val1;
    if(val1<tp_MIN)
        tp_MIN=val1;
    // top right
    int val2 = GetKValue(mTheta-CV_PI/2,topRight.x(),topRight.y());
    if(val2>tp_MAX)
        tp_MAX=val2;
    if(val2<tp_MIN)
        tp_MIN=val2;
    //bottom left
    int val3 = GetKValue(mTheta-CV_PI/2,bottomLeft.x(),bottomRight.y());
    if(val3>tp_MAX)
        tp_MAX=val3;
    if(val3<tp_MIN)
        tp_MIN=val3;
    // bottom right
    int val4 = GetKValue(mTheta-CV_PI/2,bottomRight.x(),bottomRight.y());
    if(val4>tp_MAX)
        tp_MAX=val4;
    if(val4<tp_MIN)
        tp_MIN=val4;



    std::vector<int> TpHistGram(tp_MAX-tp_MIN+1,0);

    int row_end=topLeft.y()<topRight.y()?topRight.y():topLeft.y();
    int row_start=bottomLeft.y()<bottomRight.y()?bottomLeft.y():bottomRight.y();

    for(int mrow=row_start;mrow<row_end;++mrow)
    {
        if(mrow>=0 && mrow<C_Processed_Img.rows)
            for(int mcol = 0;mcol<C_Processed_Img.cols;++mcol)
            {
                // check if is in box
                int tp = GetKValue(mTheta,mcol,mrow)-m_MIN;
                if(tp>m_low&&tp<m_high)
                {
                   unsigned char val = C_Processed_Img.at<uchar>(mrow,mcol);// this is like matrix ith row,jth col
                   if((int)val>100)
                   {
                       int tp1 = GetKValue(mTheta-CV_PI/2,mcol,mrow)-tp_MIN;
                       TpHistGram.at(tp1)++;
                   }
                }
            }
    }



/******************************* find the horizontal position  *****************************************/

    int project_h = GetKValue(mTheta-CV_PI/2,m_point.x(),m_point.y())-tp_MIN;

    // the position
    int h_pos;
    if(TpHistGram.at(project_h)==0)
    {
        int left1=project_h,left2=project_h;
        while(left1>=0 && TpHistGram[left1]==0)
            --left1;
        while(left2<TpHistGram.size()&& TpHistGram[left2]==0)
            ++left2;
        h_pos=(left1+left2)/2;
    }
    else
    {
        // left1---left2     left3--left4
        int left1=project_h,left2=project_h;
        int left3=project_h,left4=project_h;

        while(left2>=0 && TpHistGram[left2]!=0)
            --left2;
        left1=left2;
        while(left1>=0 && TpHistGram[left1]==0)
            --left1;

        while(left3<TpHistGram.size()&& TpHistGram[left3]!=0)
            ++left3;
        left4=left3;
        while(left4<TpHistGram.size()&& TpHistGram[left4]==0)
            ++left4;

        if(project_h-left2<left3-project_h)
            h_pos=(left1+left2)/2;
        else
            h_pos=(left4+left3)/2;
    }

    /*
    * project back to point;
    * solve the function
    *
    * QPoint bar_top,bar_bottom;
    *
    */


    /**
    m_high =  cos(mTheta)*x+sin(mTheta)*y-m_MIN;
    h_pos = cos(mTheta-CV_PI)*x+sin(mTheta-CV_PI)*y-tp_MIN;

    m_low =  cos(mTheta)*x+sin(mTheta)*y-m_MIN;
    h_pos = cos(mTheta-CV_PI)*x+sin(mTheta-CV_PI)*y-tp_MIN;
    */

    double a1 = cos(mTheta),a2=cos(mTheta-CV_PI/2);
    double b1 = sin(mTheta),b2 = sin(mTheta-CV_PI/2);
    double c1 = m_high+m_MIN,c2 = h_pos+tp_MIN;

    bar_top.setX((c1*b2-c2*b1)/(a1*b2-a2*b1));
    bar_top.setY((c1*a2-c2*a1)/(a2*b1-a1*b2));

    c1 = m_low+m_MIN;
    bar_bottom.setX((c1*b2-c2*b1)/(a1*b2-a2*b1));
    bar_bottom.setY((c1*a2-c2*a1)/(a2*b1-a1*b2));
}





/*
 *      s_bar_bottom    e_bar_bottom
 *          |
 *          |
 *          |
 *          |
 *          |
 *      s_bar_top       e_bar_top
 *
 */

QImage &XImageProcess::GetSelectedImg(XPostion& mPos)
{

    Crop_Img = Q_Blur_Img.copy();

    int s_top_index = GetKValue(mTheta,mPos.s_bar_top.x(),mPos.s_bar_top.y())-m_MIN;
    int s_bottom_index = GetKValue(mTheta,mPos.s_bar_bottom.x(),mPos.s_bar_bottom.y())-m_MIN;

    int e_top_index = GetKValue(mTheta,mPos.e_bar_top.x(),mPos.e_bar_top.y())-m_MIN;
    int e_bottom_index = GetKValue(mTheta,mPos.e_bar_bottom.x(),mPos.e_bar_bottom.y())-m_MIN;



    /********************** the rectangle region *************************************/
    int bottom1 = GetKValue(mTheta,mPos.s_bar_bottom.x(),mPos.s_bar_bottom.y())-m_MIN;
    int bottom2 = GetKValue(mTheta,mPos.e_bar_bottom.x(),mPos.e_bar_bottom.y())-m_MIN;

    int top1 = GetKValue(mTheta,mPos.s_bar_top.x(),mPos.s_bar_top.y())-m_MIN;
    int top2 = GetKValue(mTheta,mPos.e_bar_top.x(),mPos.e_bar_top.y())-m_MIN;

    int m_high = top2>top1?top2:top1;
    int m_low = bottom1<bottom2?bottom1:bottom2;


    QPoint topLeft,topRight,bottomLeft,bottomRight;
    // top left (0,y)
    topLeft.setX(0);
    topLeft.setY((m_high+m_MIN)*1.0/sin(mTheta));

    // top right(cols,y)
    topRight.setX(C_Processed_Img.cols);
    topRight.setY(-(cos(mTheta)*topRight.x()-m_high-m_MIN)*1.0/sin(mTheta));

    // bottom left (0,y)
    bottomLeft.setX(0);
    bottomLeft.setY((m_low+m_MIN)*1.0/sin(mTheta));

    // bottom right (cols,y)
    bottomRight.setX(C_Processed_Img.cols);
    bottomRight.setY(-(cos(mTheta)*bottomRight.x()-m_low-m_MIN)*1.0/sin(mTheta));


    // bottom left , top right
    QPoint xBottomLeft,xTopRight;
    xBottomLeft.setX(0);
    xBottomLeft.setY(bottomLeft.y()<bottomRight.y()?bottomLeft.y():bottomRight.y());

    xTopRight.setX(C_Processed_Img.cols);
    xTopRight.setY(topRight.y()>topLeft.y()?topRight.y():topLeft.y());




    for(int i=xBottomLeft.y();i<xTopRight.y();++i)
    {
        for(int j=0;j<Crop_Img.width();++j)
        {
            if(i>=0 && i< Crop_Img.height())
            {
                int tp = GetKValue(mTheta,j,i)-m_MIN;
                double x1 = mPos.s_bar_top.x()-mPos.s_bar_bottom.x();
                double y1 = mPos.s_bar_top.y()-mPos.s_bar_bottom.y();
                double x2 = j-mPos.s_bar_bottom.x();
                double y2 = i-mPos.s_bar_bottom.y();


                double x3 = j-mPos.e_bar_bottom.x();
                double y3 = i-mPos.e_bar_bottom.y();
                double x4 = mPos.e_bar_top.x()-mPos.e_bar_bottom.x();
                double y4 = mPos.e_bar_top.y()-mPos.e_bar_bottom.y();

                if(!(tp<=s_bottom_index || tp>=e_top_index ||
                        (tp<s_top_index && tp>= s_bottom_index && (x1*y2-x2*y1)>=0)||
                        (tp<= e_top_index && tp> e_bottom_index && (x3*y4-x4*y3)>=0)))
                {
                    Crop_Img.setPixelColor(j,i,Q_Origin_Img.pixelColor(j,i));
                }
            }

        }
    }

    return Crop_Img;
}





QImage XImageProcess::GetCropedImg(XPostion& mPos)
{

    GetSelectedImg(mPos);

    int bottom1 = GetKValue(mTheta,mPos.s_bar_bottom.x(),mPos.s_bar_bottom.y())-m_MIN;
    int bottom2 = GetKValue(mTheta,mPos.e_bar_bottom.x(),mPos.e_bar_bottom.y())-m_MIN;

    int top1 = GetKValue(mTheta,mPos.s_bar_top.x(),mPos.s_bar_top.y())-m_MIN;
    int top2 = GetKValue(mTheta,mPos.e_bar_top.x(),mPos.e_bar_top.y())-m_MIN;

    int m_high = top2>top1?top2:top1;
    int m_low = bottom1<bottom2?bottom1:bottom2;


    QPoint topLeft,topRight,bottomLeft,bottomRight;
    // top left (0,y)
    topLeft.setX(0);
    topLeft.setY((m_high+m_MIN)*1.0/sin(mTheta));

    // top right(cols,y)
    topRight.setX(C_Processed_Img.cols);
    topRight.setY(-(cos(mTheta)*topRight.x()-m_high-m_MIN)*1.0/sin(mTheta));

    // bottom left (0,y)
    bottomLeft.setX(0);
    bottomLeft.setY((m_low+m_MIN)*1.0/sin(mTheta));

    // bottom right (cols,y)
    bottomRight.setX(C_Processed_Img.cols);
    bottomRight.setY(-(cos(mTheta)*bottomRight.x()-m_low-m_MIN)*1.0/sin(mTheta));


    QPoint xBottomLeft,xTopRight;
    xBottomLeft.setX(0);
    xBottomLeft.setY(bottomLeft.y()<bottomRight.y()?bottomLeft.y():bottomRight.y());

    xTopRight.setX(C_Processed_Img.cols);
    xTopRight.setY(topRight.y()>topLeft.y()?topRight.y():topLeft.y());

    QImage &tp = Crop_Img.copy(QRect(xBottomLeft,xTopRight));

    return tp;
}














/**
 * @brief XImageProcess::GetKValue
 * @param theta
 * @param x the x cord
 * @param y the y cord
 * @return
 */
int XImageProcess::GetKValue(float theta,int x,int y)
{
    return int(cos(theta)*x+sin(theta)*y);
}








void XImageProcess::Project2VerticalLine(float theta)
{
    int m_row=C_Processed_Img.rows,m_col=C_Processed_Img.cols;

    m_MIN = 1;
    m_MAX = -1;

    // top left
    int val1 = GetKValue(theta,0,0);
    if(val1>m_MAX)
        m_MAX=val1;
    if(val1<m_MIN)
        m_MIN=val1;
    // top right
    int val2 = GetKValue(theta,m_col-1,0);
    if(val2>m_MAX)
        m_MAX=val2;
    if(val2<m_MIN)
        m_MIN=val2;
    //bottom left
    int val3 = GetKValue(theta,0,m_row-1);
    if(val3>m_MAX)
        m_MAX=val3;
    if(val3<m_MIN)
        m_MIN=val3;
    // bottom right
    int val4 = GetKValue(theta,m_col-1,m_row-1);
    if(val4>m_MAX)
        m_MAX=val4;
    if(val4<m_MIN)
        m_MIN=val4;

    VHistGram.clear();
    VHistGram.assign(m_MAX-m_MIN+1,0);

    for(int i=0;i<C_Processed_Img.rows;++i)
    {
        for(int j=0;j<C_Processed_Img.cols;++j)
        {
            unsigned char val = C_Processed_Img.at<uchar>(i,j);// this is like matrix ith row,jth col
            if((int)val>100)
            {
                int t = GetKValue(theta,j,i);// x cord is j,y cord is i
                VHistGram.at(t-m_MIN)++;
            }
        }
    }
}


cv::Mat XImageProcess::QImage2cvMat(QImage image)
{
    cv::Mat mat;
    //qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}





