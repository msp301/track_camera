#include "VideoBuffer.hpp"

#include <QMutexLocker>

#include <iostream>

VideoBuffer::VideoBuffer()
{
}

//insert frame onto end of buffer
void VideoBuffer::add( cv::Mat frame )
{
    //create mutex locker to enforce mutual exclusion
    //QMutexLocker locker( mutex );
    cout << "VideoBuffer: Adding new frame" << endl;
    buffer.push_back( frame ); //push frame onto end of buffer
}

//access frame from front of buffer
cv::Mat VideoBuffer::read()
{
    //create mutex locker to enforce mutual exclusion
    //QMutexLocker locker( mutex );
    cout << "VideoBuffer: Reading frame" << endl;
    cv::Mat frame = buffer.front(); //return frame on the front of the buffer
    buffer.erase( buffer.begin() ); //remove frame from front of buffer
    return frame;
}
