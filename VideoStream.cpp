#include "VideoStream.hpp"

VideoStream::VideoStream( VideoBuffer *buffer )
{
    video_buffer = buffer; //store video buffer location
    create(); //create new capture interface
}

//start video capturing thread execution
void VideoStream::start()
{
    capture->start(); //start capturing video in dedicated thread
}

//pause video capturing thread execution
void VideoStream::stop()
{
    capture->wait(); //pause capture thread
}

//create new capture instance
bool VideoStream::create()
{
    bool success = true;

    //check whether capture interface is currently defined
    if( capture == NULL )
    {
        capture = new Capture( video_buffer );
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
