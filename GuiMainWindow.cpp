#include "GuiMainWindow.hpp"
#include "ui_GuiMainWindow.h"

#include <QtGui>

#include <opencv2/imgproc/imgproc.hpp>

GuiMainWindow::GuiMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle( "Track Camera" );

    video_buffer = new VideoBuffer; //create video buffer for captured frames
    video_stream = new VideoStream( video_buffer ); //create video stream handler
    video_display = new DisplayStream( video_buffer ); //create display handler

    face_tracking = new FaceTracking( video_buffer ); //create face tracking handler

    createConnections(); //setup UI callback connections
}

GuiMainWindow::~GuiMainWindow()
{
	delete ui;
}

//create all required connections between GUI elements
void GuiMainWindow::createConnections()
{
    createMenuConnections(); //setup menu connections

    //define cv::Mat as a Qt type so that it can be used within a signal
    qRegisterMetaType<cv::Mat>( "cv::Mat" );

    //connect DisplayStream thread to camera output label on UI
    connect( video_display, SIGNAL( frameReady( cv::Mat ) ),
             this, SLOT( displayFrame( cv::Mat ) ) );

    //connect FaceTracking thread to camera output label on UI
    connect( face_tracking, SIGNAL( frameReady( cv::Mat ) ),
             this, SLOT( displayFrame( cv::Mat ) ) );
}

//create required connections for menu actions
void GuiMainWindow::createMenuConnections()
{
    // ----- Camera menu -----

    //Camera > Start = start video
    connect( ui->actionStart, SIGNAL( triggered() ),
             this, SLOT( displayVideo() ) );

    //Camera > Quit = exit application
    connect( ui->actionQuit, SIGNAL( triggered() ), qApp, SLOT( quit() ) );
}

//start reading video output stream
void GuiMainWindow::displayVideo()
{
    video_stream->start(); //start video stream
    face_tracking->start(); //start tracking faces
    //video_display->start(); //start reading from video buffer
}

//display video frame to interface
void GuiMainWindow::displayFrame( cv::Mat frame )
{
    cv::Mat conv_frame;
    cv::cvtColor( frame, conv_frame, CV_BGR2RGB ); //convert to RGB colour

    //create new QImage from OpenCV matrix format
    QImage image( ( uchar* ) conv_frame.data, conv_frame.cols,
                  conv_frame.rows, QImage::Format_RGB888 );

    //display converted frame to UI label
    ui->lbl_camera_output->setPixmap( QPixmap::fromImage( image ) );
}
