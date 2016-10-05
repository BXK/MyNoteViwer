#ifndef XIMAGEPROCESS_H
#define XIMAGEPROCESS_H
#include<QImage>
#include"cv.h"
#include"cvaux.h"
#include"highgui.h"
#include "QDebug"
#include<QImage>

#include "xblur.h"



class XPostion
{
public:
    QPoint s_bar_top,s_bar_bottom;
    QPoint e_bar_top,e_bar_bottom;

    XPostion GetScale(float scaleing)
    {
        XPostion tp;
        tp.s_bar_top=s_bar_top*scaleing;
        tp.s_bar_bottom=s_bar_bottom*scaleing;
        tp.e_bar_top=e_bar_top*scaleing;
        tp.e_bar_bottom=e_bar_bottom*scaleing;

        return tp;
    }




};





class XImageProcess
{
public:
    XImageProcess();

    /**
     * @brief Process, preprocess the image
     * @param qimg
     */
    void Process(QImage qimg, QString filepath, bool flag);

    /**
     * @brief GetBar get the bar positon the mouse point
     * @param m_point
     * @param Start_Flag
     * @param bar_top   y is bigger
     * @param bar_bottom    y is small
     */
    void GetBar(QPoint m_point,bool Start_Flag,QPoint &bar_top,QPoint &bar_bottom);




    /**
     * @brief GetSelectedImg get the selected image with blur outside
     * @param s_bar_top
     * @param s_bar_bottom
     * @param e_bar_top
     * @param e_bar_bottom
     * @return
     */
    QImage & GetSelectedImg(XPostion& mPos);



    /**
     * @brief GetCorpdImg get Croped Image with selected region
     * @param s_bar_top
     * @param s_bar_bottom
     * @param e_bar_top
     * @param e_bar_bottom
     * @return
     */
    QImage GetCropedImg(XPostion& mPos);





private:
    /**
     * @brief QImage2cvMat convert the qimage
     * @param image
     * @return
     */
    cv::Mat QImage2cvMat(QImage image);


    /**
     * @brief Project2VerticalLine project the paragraph to the vertical line
     * @param theta
     */
    void Project2VerticalLine(float theta);


    /**
     * @brief GetKValue project the x,y to line with theta degree
     * @param theta
     * @param x
     * @param y
     * @return
     */
    int GetKValue(float theta, int x, int y);




    void SaveData(QString filepath);
    bool LoadData(QString filepath);


private:        
    // the origin Img
    QImage Q_Origin_Img;

    //Processed Img
    QImage Q_Processed_Img;
    cv::Mat C_Processed_Img;

    // for blur the source image
    QImage  Q_Blur_Img;
    cv::Mat C_Blur_Img;


    QImage Crop_Img;

    XBlur m_Blur;


    std::vector<int> VHistGram;
    double mTheta;
    int m_MIN;
    int m_MAX;
};

#endif // XHOUGH_H
