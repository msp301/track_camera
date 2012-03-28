#include "VideoBuffer.hpp"

#include <QDebug>

//#include <QMutexLocker>

VideoBuffer::VideoBuffer()
{
    semaphore =  new QSemaphore( 1 ); //initialise semaphore with 1 token
}

//insert frame onto end of buffer
void VideoBuffer::add( cv::Mat frame )
{
    //create mutex locker to enforce mutual exclusion
    //QMutexLocker locker( mutex );
    //semaphore->tryAcquire( 1 );
    buffer.push_back( frame ); //push frame onto end of buffer
    //semaphore->release( 1 );
}

//access frame from front of buffer
cv::Mat VideoBuffer::read()
{
    //create mutex locker to enforce mutual exclusion
    //QMutexLocker locker( mutex );
    //semaphore->tryAcquire( 1 );
    qDebug() << "Buffer size = " << buffer.size();
    cv::Mat frame;
    if( buffer.size() > 0 )
    {
        frame = buffer.front(); //return frame on the front of the buffer
    }
    //semaphore->release( 1 );
    return frame;
}
