#include "VideoBuffer.hpp"

VideoBuffer::VideoBuffer()
{
    buffer = new cv::Mat[1000]; //set size of video buffer
    buffer_head = 0; //set head of buffer
    buffer_tail = 0; //set tail of buffer
    buffer_size = 0; //count number of elements on buffer
    mutex = new QMutex;
}

//insert frame onto end of buffer
void VideoBuffer::add( cv::Mat frame )
{
    //buffer.push_back( frame ); //push frame onto end of buffer
    if( !frame.empty() )
    {
        mutex->lock();
        buffer[ buffer_tail ] = frame;
        buffer_tail = ( ( buffer_tail + 1 ) == buffer_size ) ? 0 : buffer_tail + 1;
        buffer_size = sizeof( buffer );
        mutex->unlock();
    }
}

//access frame from front of buffer
cv::Mat VideoBuffer::read()
{
    cv::Mat frame;

    //ensure data exists in the buffer before reading
    if( buffer_size > 0 )
    {
        //frame = buffer.front(); //return frame on the front of the buffer
        mutex->lock();
        frame = buffer[ buffer_head ];
        buffer_head = ( ( buffer_head + 1 ) == buffer_size ) ? 0 : buffer_head + 1;
        mutex->unlock();
    }

    return frame;
}
