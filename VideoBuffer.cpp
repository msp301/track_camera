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
    return buffer.front();
}
