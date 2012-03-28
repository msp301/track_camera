#include "Capture.hpp"

#include <opencv2/imgproc/imgproc.hpp>

Capture::Capture( cv::VideoCapture *camera, VideoBuffer *buffer )
{
    video_buffer = buffer; //store reference to video buffer
    capture = camera; //store reference to camera device
}

Capture::~Capture()
{
    capture->release(); //disconnect camera before closing program
}

//thread implementation to start video stream capturing
void Capture::run()
{
    while( true )
    {
        cv::Mat frame = grabFrame(); //grab frame from video device
        video_buffer->add( frame ); //add frame to video buffer
        msleep( 60 );
    }
}

//grab current frame from input capture device
cv::Mat Capture::grabFrame()
{
    cv::Mat frame;

    capture->operator >>( frame ); //capture frame from video device

    return frame;
}


