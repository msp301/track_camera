#include "VideoBuffer.hpp"

VideoBuffer::VideoBuffer()
{
    buffer[1000]; //set size of video buffer
    buffer_head = 0; //set head of buffer
    buffer_tail = 0; //set tail of buffer
    buffer_size = 0; //count number of elements on buffer
    sem = new QSemaphore( 1 );
}

//insert frame onto end of buffer
void VideoBuffer::add( cv::Mat frame )
{
    //buffer.push_back( frame ); //push frame onto end of buffer
    sem->acquire( 1 );
    buffer[ buffer_tail ] = frame;
    if( ( buffer_tail + 1 ) == buffer_size ) buffer_tail++;
    sem->release( 1 );
}

//access frame from front of buffer
cv::Mat VideoBuffer::read()
{
    cv::Mat frame;

    //ensure data exists in the buffer before reading
    if( buffer_size > 0 )
    {
        //frame = buffer.front(); //return frame on the front of the buffer
        sem->acquire( 1 );
        frame = buffer[ buffer_head ];
        buffer_head++;
        sem->release( 1 );
    }

    return frame;
}
