#include "DisplayStream.hpp"

DisplayStream::DisplayStream( VideoBuffer *buffer )
{
    video_buffer = buffer;
}

void DisplayStream::run()
{
    while( true )
    {
        msleep( 120 );
        cv::Mat frame = video_buffer->read();
        if( !frame.empty() ) emit( frameReady( frame ) );
    }
}
