#include "DisplayStream.hpp"

#include <QDebug>

DisplayStream::DisplayStream( VideoBuffer *buffer )
{
    video_buffer = buffer; //store reference to given video buffer
    mutex = new QMutex; //create new mutex
}

DisplayStream::~DisplayStream()
{
    this->quit(); //stop thread execution
}

//display video stream thread implementation
void DisplayStream::run()
{
    exec(); //enter thread wait routine
}

//change the input video buffer to read frames from
void DisplayStream::setVideoBuffer( VideoBuffer *buffer )
{
    mutex->lock();
    video_buffer = buffer; //change video stream reference
    mutex->unlock();
}

//retrieve current video frame for displaying
void DisplayStream::displayVideoFrame()
{
    mutex->lock();
    cv::Mat frame = video_buffer->read(); //read frame from video buffer
    mutex->unlock();

    //ensure retrieved frame contains some data before signalling to UI
    if( !frame.empty() ) emit( frameReady( frame ) );
}
