#include "GuiMainWindow.hpp"
#include "ui_GuiMainWindow.h"

#include "Capture.hpp"

#include <QtGui>

#include <opencv2/imgproc/imgproc.hpp>

GuiMainWindow::GuiMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::GuiMainWindow)
{
	ui->setupUi(this);

    video_buffer = new VideoBuffer; //create video buffer for captured frames
    video_stream = new VideoStream( video_buffer ); //create video stream handler
    video_display = new DisplayStream( video_buffer ); //create display handler

    createConnections(); //setup UI callback connections

    //QPushButton *btn_start_video = new QPushButton;
}

GuiMainWindow::~GuiMainWindow()
{
	delete ui;
}

//create all required connections between GUI elements
void GuiMainWindow::createConnections()
{
    connect( ui->btn_start_video, SIGNAL( clicked() ),
             this, SLOT( displayVideo() ) );

    qRegisterMetaType<cv::Mat>( "cv::Mat" );

    //connect DisplayStream thread to camera output label on UI
    connect( video_display, SIGNAL( frameReady( cv::Mat ) ),
             this, SLOT( displayFrame( cv::Mat ) ) );
}

//start reading video output stream
void GuiMainWindow::displayVideo()
{
    video_stream->start(); //start video stream
    //cv::Mat frame = video_buffer->read();
    video_display->start(); //start reading from video buffer
}

//display video frame to interface
void GuiMainWindow::displayFrame( cv::Mat frame )
{
    qDebug() << "displayFrame: attempting to display image";
    cv::Mat conv_frame;
    cv::cvtColor( frame, conv_frame, CV_BGR2RGB ); //convert to RGB colour
    QImage image( ( uchar* ) conv_frame.data, conv_frame.cols,
                  conv_frame.rows, QImage::Format_RGB888 );
    ui->lbl_camera_output->setPixmap( QPixmap::fromImage( image ) );
}
