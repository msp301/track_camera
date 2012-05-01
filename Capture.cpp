#include "Capture.hpp"

#include <QDebug>

#include <opencv2/imgproc/imgproc.hpp>

Capture::Capture( cv::VideoCapture *camera, VideoBuffer *buffer )
{
    video_buffer = buffer; //store reference to video buffer
    capture = camera; //store reference to camera device
}

Capture::~Capture()
{
    this->quit(); //quit thread
}

//thread implementation to start video stream capturing
void Capture::run()
{
    exec(); //enter thread wait routine
}

//grab current frame from input capture device
cv::Mat Capture::grabFrame()
{
    cv::Mat frame;

    //capture frame from video device, only if it exists
    if( capture ) capture->operator >>( frame );

    return frame;
}

//capture video frame
void Capture::captureFrame()
{
    cv::Mat frame = grabFrame(); //grab frame from video device

    //do not write empty frames to video buffer
    if( !frame.empty() )
    {
        video_buffer->add( frame ); //add frame to video buffer
    }
}
