#include "VideoStream.hpp"

VideoStream::VideoStream( VideoBuffer *buffer )
{
    video_buffer = buffer; //store video buffer location
    camera = new cv::VideoCapture( 0 ); //create link to default camera
    create(); //create new capture interface
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
    if( capture->isRunning() ) capture->wait();
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
        delete capture; //delete capture interface instance
    }
    else
    {
        //do not delete a capture interface if it has not been created
        success = false;
    }

    return success;
}
