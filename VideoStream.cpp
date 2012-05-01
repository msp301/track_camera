#include "VideoStream.hpp"

VideoStream::VideoStream( VideoBuffer *buffer )
{
    video_buffer = buffer; //store video buffer location
    camera = new cv::VideoCapture( 0 ); //create link to default camera
    capture = NULL; //ensure capture interface is empty
    create(); //create new capture interface
}

VideoStream::~VideoStream()
{
    remove(); //remove video capture interface
    delete capture;
}

//start video capturing thread execution
void VideoStream::start()
{
    //start capturing video in dedicated thread only if capture has not
    //already been started and the camera device is ready for reading
    if( !capture->isRunning() &&
            camera->isOpened() ) capture->start();
}

//pause video capturing thread execution
void VideoStream::stop()
{
    //pause capture thread if running
    if( capture->isRunning() ) capture->quit();
}

//create new capture instance
bool VideoStream::create()
{
    bool success = true;

    //check whether capture interface is currently defined
    if( capture == NULL )
    {
        //setup capture from camera that will be output to a video buffer
        capture = new Capture( camera, video_buffer );

        timer_capture = new QTimer( capture ); //setup capture event timer
        connect( timer_capture, SIGNAL( timeout() ),
                 capture, SLOT( captureFrame() ) );
        timer_capture->start( 60 ); //execute capture every 60ms
    }
    else
    {
        //do not overwrite capture interface when already defined
        success = false;
    }

    return success;
}

//remove video capturing thread
bool VideoStream::remove()
{
    bool success = true;

    //check whether capture interface exists before creating
    if( capture != NULL )
    {
        if( capture->isRunning() ) capture->quit(); //stop capture thread
        capture = NULL; //set capture to empty
    }
    else
    {
        //do not delete a capture interface if it has not been created
        success = false;
    }

    return success;
}

//set camera device
void VideoStream::setCamera( int camera_no )
{
    if( capture != NULL ) stop(); //stop any running video capture
    remove(); //remove link to capture task
    delete capture; //remove capture task

    if( camera->isOpened() ) camera->release(); //release any connected camera
    delete camera; //remove camera instance

    camera = new cv::VideoCapture( camera_no ); //setup new camera
    create(); //create new capture interface
}
