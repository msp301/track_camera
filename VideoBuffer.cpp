#include "VideoBuffer.hpp"

VideoBuffer::VideoBuffer()
{
}

//insert frame onto end of buffer
void VideoBuffer::add( cv::Mat frame )
{
    buffer.push_back( frame ); //push frame onto end of buffer
}

//access frame from front of buffer
cv::Mat VideoBuffer::read()
{
    cv::Mat frame;

    //ensure data exists in the buffer before reading
    if( buffer.size() > 0 )
    {
        frame = buffer.front(); //return frame on the front of the buffer
    }

    return frame;
}
