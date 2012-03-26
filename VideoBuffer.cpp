#include "VideoBuffer.hpp"

#include <QMutexLocker>

VideoBuffer::VideoBuffer()
{
}

//insert frame onto end of buffer
void VideoBuffer::add( cv::Mat frame )
{
    //create mutex locker to enforce mutual exclusion
    QMutexLocker locker( mutex );
    buffer.push_back( frame ); //push frame onto end of buffer
}

//access frame from front of buffer
cv::Mat VideoBuffer::read()
{
    //create mutex locker to enforce mutual exclusion
    QMutexLocker locker( mutex );
    return buffer.front(); //return frame on the front of the buffer
}
