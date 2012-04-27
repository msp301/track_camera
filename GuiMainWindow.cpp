#include "GuiMainWindow.hpp"
#include "ui_GuiMainWindow.h"

#include <QtGui>

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

GuiMainWindow::GuiMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle( "Track Camera" );

    QString cap_buffer( "capture" );

    video_buffer = new VideoBuffer( cap_buffer ); //create video buffer for captured frames
    video_stream = new VideoStream( video_buffer ); //create video stream handler

    stand = new StandController; //create new stand control handler

    face_tracking = new FaceTracking( video_buffer, stand ); //create face tracking handler

    video_display = new DisplayStream( video_buffer ); //create display handler

    setupGui(); //setup window elements
    setupTimers();
    createConnections(); //setup UI callback connections
}

GuiMainWindow::~GuiMainWindow()
{
    //video_display->stopThread();
    //face_tracking->stopThread();
    //while( !video_display->isStopped() );
    //if( video_display->isStopped() ) qDebug() << "Stopped Video Display";

    //delete video_display;
    //while( !face_tracking->isStopped() );
    //if( face_tracking->isStopped() ) qDebug() << "Stopped Face Tracking";

    delete video_stream;
    delete video_buffer;
    delete stand;

    //video_display->quit();
    //face_tracking->quit();

    delete video_display;
    //delete face_tracking;

    delete ui;
}

//setup GUI elements
void GuiMainWindow::setupGui()
{
    ui->cmb_device->addItem( "Select Device" ); //add default entry to device list

    //add available devices to device list
    foreach( QextPortInfo port, stand->availablePorts() )
    {
        ui->cmb_device->addItem( port.physName ); //add device to combo box
    }
}

//setup event timers
void GuiMainWindow::setupTimers()
{
    timer_video_display = new QTimer( video_display );
    connect( timer_video_display, SIGNAL( timeout() ),
             video_display, SLOT( displayVideoFrame() ) );

    timer_face_tracking = new QTimer( face_tracking );
    connect( timer_face_tracking, SIGNAL( timeout() ),
             face_tracking, SLOT( trackFaces() ) );
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

    //connect device combo box to selection of stand controller
    connect( ui->cmb_device, SIGNAL( currentIndexChanged( QString ) ),
             this, SLOT( setDevice( QString ) ) );
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

    // ----- Edit menu -----

    //Edit > Show Faces = display face identification
    connect( ui->actionShow_Faces, SIGNAL( triggered() ),
             this, SLOT( toggleDisplayFaces() ) );
}

//start reading video output stream
void GuiMainWindow::displayVideo()
{
    video_stream->start(); //start video stream
    timer_face_tracking->start( 250 ); //start scheduling face tracking
    timer_video_display->start( 250 ); //start scheduling video buffer
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

//change video buffer to be used as output
void GuiMainWindow::toggleDisplayFaces()
{
    //set menu item text appropriate to request
    if( face_tracking->showDetectedFaces() )
    {
        ui->actionShow_Faces->setText( "Show Faces" );
    }
    else
    {
        ui->actionShow_Faces->setText( "Hide Faces" );
    }

    //toggle whether to display detected faces or not
    face_tracking->toggleDisplayDetectedFaces();
}

//set stand device
void GuiMainWindow::setDevice( QString device_name )
{
    qDebug() << "About to Set Device";
    stand->setPort( device_name ); //set stand device
}
