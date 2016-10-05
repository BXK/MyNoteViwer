#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"XImageViewer.h"
#include <QAction>
#include<QMenu>
#include<QToolBar>
#include<QMenuBar>
#include <QFileDialog>
#include <QStringList>
#include <QScrollArea>
#include <QSlider>
#include <QPushButton>
#include <QSpinBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void creatConnect();

private:

    void keyPressEvent(QKeyEvent *event);


    QStringList FileNameList;

    XImageViewer m_Viewer;


    QAction *OpenFiles;

    // status bar
    QSlider *ScaleSlider;
    QPushButton *ScaleWidthFit;


    QSlider *PageSlider;
    QSpinBox *PageSpin;




    QString mFilePath;// the filefolder path

public slots:
    void openFiles_slot();
    void changeScale_slot(int val);
    void changeWidthFit_slot();
    void setPage_slot(int val);
    void add_page_slot()
    {

        int val = PageSlider->value();
        if(val<FileNameList.size())
        {
            val++;
            PageSlider->setValue(val);
        }
    }

    void reduce_page_slot()
    {
        int val = PageSlider->value();
        if(val>0)
        {
            val--;
            PageSlider->setValue(val);
        }

    }

};

#endif // MAINWINDOW_H
