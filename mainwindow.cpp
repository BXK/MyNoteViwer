#include "MainWindow.h"
#include <QDesktopWidget>
#include <QApplication>

#include <QStatusBar>
#include <QHBoxLayout>
#include <QSplitter>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    //setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("PicViewr");



    // for key listening
    setFocusPolicy(Qt::StrongFocus);


    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    resize(desktop->width()/3*2,desktop->height()/3*2);
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);


    OpenFiles = new QAction(tr("&Open"),this);


    QMenu *FileMenu = menuBar()->addMenu(tr("&File"));
    FileMenu->addAction(OpenFiles);


    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(OpenFiles);



    QScrollArea *view = new QScrollArea;
    //view->setFocusPolicy(Qt::NoFocus);
    //view->setFrameStyle(QFrame::NoFrame);
    //view->show();


    view->setWidget(&m_Viewer);
    view->setWidgetResizable(true);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    this->setCentralWidget(view);


    ScaleSlider = new QSlider(Qt::Horizontal);
    ScaleSlider->setMaximum(100);
    ScaleSlider->setMinimum(1);
    ScaleSlider->setValue(50);


    PageSlider = new QSlider(Qt::Horizontal);
    PageSlider->setMaximum(100);
    PageSlider->setMinimum(0);
    PageSlider->setValue(0);

    PageSpin = new QSpinBox;
    PageSpin->setMaximum(100);
    PageSpin->setMinimum(0);
    PageSpin->setValue(0);


    ScaleWidthFit= new QPushButton("WidthFit");



    QWidget *BottomStatusBar = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(ScaleSlider,1);
    hbox->addWidget(ScaleWidthFit);
    hbox->addStretch(1);
    hbox->addWidget(PageSlider,1);
    hbox->addWidget(PageSpin);
    BottomStatusBar->setLayout(hbox);

    statusBar()->addWidget(BottomStatusBar,1);

    creatConnect();
}

MainWindow::~MainWindow()
{

}

void MainWindow::creatConnect()
{
    connect(OpenFiles,SIGNAL(triggered(bool)),this,SLOT(openFiles_slot()));

    connect(this->ScaleSlider,SIGNAL(valueChanged(int)),this,SLOT(changeScale_slot(int)));
    connect(this->ScaleWidthFit,SIGNAL(clicked(bool)),this,SLOT(changeWidthFit_slot()));
    connect(PageSlider,SIGNAL(valueChanged(int)),PageSpin,SLOT(setValue(int)));
    connect(PageSpin,SIGNAL(valueChanged(int)),PageSlider,SLOT(setValue(int)));

    connect(PageSlider,SIGNAL(valueChanged(int)),this,SLOT(setPage_slot(int)));

    connect(&m_Viewer,SIGNAL(addPage()),this,SLOT(add_page_slot()));
    connect(&m_Viewer,SIGNAL(reducePage()),this,SLOT(reduce_page_slot()));
}

void MainWindow::setPage_slot(int val)
{
    m_Viewer.SetImg(FileNameList[val]);
}


void MainWindow::changeScale_slot(int val)
{
    m_Viewer.SetScale(tan((val-50)*1.0/50)+1);
}

void MainWindow::changeWidthFit_slot()
{
    float tp = m_Viewer.GetWidthFitScale(this->width()-40);

    ScaleSlider->setValue(atan(tp-1)*50+50);
}





void MainWindow::openFiles_slot()
{
    QString filepath = QFileDialog::getOpenFileName(this,"Open Index",QDir::currentPath(),
                                      "book Index(*.book);");

    FileNameList.clear();

    if(!filepath.isEmpty())
    {
        QFileInfo fi = QFileInfo(filepath);
        mFilePath = fi.absolutePath();

        QDir xImag;
        //qDebug()<<mFilePath+"/ImgData";
        if(!xImag.exists(mFilePath+"/ImgData"))
        {
            xImag.mkdir(mFilePath+"/ImgData");
        }
        if(!xImag.exists(mFilePath+"/ImgData/db"))
        {
            xImag.mkdir(mFilePath+"/ImgData/db");
        }

        if(!xImag.exists(mFilePath+"/ImgData/pic"))
        {
            xImag.mkdir(mFilePath+"/ImgData/pic");
        }



        QFile filetp(filepath);
        if(!filetp.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<"Can't open the file!"<<endl;
        }
        while(!filetp.atEnd()) {
            QByteArray line = filetp.readLine();
            QString str(line);
            str.replace(QRegExp("\n"),"");
            FileNameList.push_back(mFilePath+"/"+str);
        }
        m_Viewer.SetImg(FileNameList[0]);



        PageSlider->setMaximum(FileNameList.size()-1);
        PageSlider->setMinimum(0);

        PageSpin->setMaximum(FileNameList.size()-1);
        PageSpin->setMinimum(0);



    }

}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_A)
    {
        reduce_page_slot();

    }
    else if(event->key()==Qt::Key_D)
    {
        add_page_slot();
    }


}
