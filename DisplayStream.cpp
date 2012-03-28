#include "DisplayStream.hpp"

DisplayStream::DisplayStream( VideoBuffer *buffer )
{
    video_buffer = buffer; //store reference to given video buffer
}

//display video stream thread implementation
void DisplayStream::run()
{
    while( true )
    {
        msleep( 120 );
        cv::Mat frame = video_buffer->read(); //read frame from video buffer

        //ensure retrieved frame contains some data before signalling to UI
        if( !frame.empty() ) emit( frameReady( frame ) );
    }
}
