#include "GuiMainWindow.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

GuiMainWindow::GuiMainWindow(QWidget *parent) : QMainWindow(parent)
{
    QString cap_buffer( "capture" );

    video_buffer = new VideoBuffer( cap_buffer ); //create video buffer for captured frames
    video_device = new VideoDevice();
    video_device->getDeviceNumber( video_device->getDeviceList().at( 0 ) );
    video_stream = new VideoStream( video_buffer ); //create video stream handler

    stand = new StandController; //create new stand control handler

    face_tracking = new FaceTracking( video_buffer, stand ); //create face tracking handler

    video_display = new DisplayStream( video_buffer ); //create display handler

    setupGui(); //setup window elements
    setupTimers(); //setup event timers
    createConnections(); //setup UI callback connections
}

GuiMainWindow::~GuiMainWindow()
{
    delete video_stream;
    delete video_buffer;
    delete stand;
    delete video_display;
    delete face_tracking;
}

//setup GUI elements
void GuiMainWindow::setupGui()
{
    content = new QWidget; //create window contents widget
    setWindowTitle( "Track Camera" ); //set window title

    //create required layouts
    v_layout = new QVBoxLayout; //top-level vertical box layout
    h_layout = new QHBoxLayout; //horizontal layout for controls

    //create "Camera" menu
    menu_camera = new QMenu( "Camera" );
    action_start = new QAction( "Start", menu_camera );
    action_quit = new QAction( "Quit", menu_camera );
    menu_camera->addAction( action_start );
    menu_camera->addSeparator();
    menu_camera->addAction( action_quit );
    menuBar()->addMenu( menu_camera );

    //create "Edit" menu
    menu_edit = new QMenu( "Edit" ); //create edit menu
    action_show_faces = new QAction( "Show Faces", menu_edit );
    menu_edit->addAction( action_show_faces );
    menuBar()->addMenu( menu_edit );

    //create stand controller and camera selection controls
    cmb_camera_select = new QComboBox;
    cmb_device_select = new QComboBox;

    cmb_camera_select->addItem( "Select Camera" ); //add default entry to camera list
    foreach( string device, video_device->getDeviceList() )
    {
        QString name = video_device->getDeviceName( device ).c_str();
        QString location = video_device->getDeviceFile( device ).c_str();

        name.append( " - " + location ); //append device name with its location
        cmb_camera_select->addItem( name ); //add camera entry to combo box
    }

    //populate stand controller device selection
    cmb_device_select->addItem( "Select Device" ); //add default entry to device list
    foreach( QextPortInfo port, stand->availablePorts() )
    {
        cmb_device_select->addItem( port.physName ); //add device to combo box
    }

    //add control widgets to horizontal control layout
    h_layout->addWidget( cmb_camera_select );
    h_layout->addWidget( cmb_device_select );

    v_layout->addLayout( h_layout ); //add controls to top-level layout

    //create video output label & add to top-level layout
    lbl_camera_output = new QLabel( "Camera Output" );
    v_layout->addWidget( lbl_camera_output );

    content->setLayout( v_layout ); //apply created layout to window

    setCentralWidget( content ); //add content to window
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
    connect( cmb_device_select, SIGNAL( currentIndexChanged( QString ) ),
             this, SLOT( setDevice( QString ) ) );

    //connect camera combo box to selection of camera device
    connect( cmb_camera_select, SIGNAL( currentIndexChanged( QString ) ),
             this, SLOT( setCamera( QString ) ) );
}

//create required connections for menu actions
void GuiMainWindow::createMenuConnections()
{
    // ----- Camera menu -----

    //Camera > Start = start video
    connect( action_start, SIGNAL( triggered() ),
             this, SLOT( displayVideo() ) );

    //Camera > Quit = exit application
    connect( action_quit, SIGNAL( triggered() ), qApp, SLOT( quit() ) );

    // ----- Edit menu -----

    //Edit > Show Faces = display face identification
    connect( action_show_faces, SIGNAL( triggered() ),
             this, SLOT( toggleDisplayFaces() ) );
}

//start reading video output stream
void GuiMainWindow::displayVideo()
{
    //check whether video output is being displayed
    if( !timer_video_display->isActive() )
    {
        action_start->setText( "Stop" ); //set UI menu text

        video_stream->start(); //start video stream
        timer_face_tracking->start( 250 ); //start scheduling face tracking
        timer_video_display->start( 250 ); //start scheduling video buffer
    }
    else
    {
        action_start->setText( "Start" ); //set UI menu text

        video_stream->stop(); //stop video stream
        timer_face_tracking->stop(); //stop scheduling face tracking
        timer_video_display->stop(); //stop scheduling video buffer
    }
}

//display video frame to interface
void GuiMainWindow::displayFrame( cv::Mat frame )
{
    cv::Mat conv_frame;

    //ensure we have received a valid video frame before displaying
    if( !frame.empty() )
    {
        cv::cvtColor( frame, conv_frame, CV_BGR2RGB ); //convert to RGB colour

        //create new QImage from OpenCV matrix format
        QImage image( ( uchar* ) conv_frame.data, conv_frame.cols,
                      conv_frame.rows, QImage::Format_RGB888 );

        //display converted frame to UI label
        lbl_camera_output->setPixmap( QPixmap::fromImage( image ) );
    }
}

//change video buffer to be used as output
void GuiMainWindow::toggleDisplayFaces()
{
    //set menu item text appropriate to request
    if( face_tracking->showDetectedFaces() )
    {
        action_show_faces->setText( "Show Faces" );
    }
    else
    {
        action_show_faces->setText( "Hide Faces" );
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

//set camera device
void GuiMainWindow::setCamera( QString camera_name )
{
    //extract device file path from selected camera name
    camera_name = camera_name.split( "-" ).back().trimmed();

    //retrieve system location of selected camera device
    string camera = video_device->deviceFileToPath( camera_name.toStdString() );

    //if video display has already started, stop video
    if( timer_video_display->isActive() )
    {
        displayVideo(); //toggle video display
        for( int i=-0; i < 20000; i++ ); //wait for tasks to stop properly

        //retrieve camera device number and set video stream to use that device
        video_stream->setCamera( video_device->getDeviceNumber( camera ) );

        displayVideo(); //toggle video display
    }
    else
    {
        //retrieve camera device number and set video stream to use that device
        video_stream->setCamera( video_device->getDeviceNumber( camera ) );
    }

}
