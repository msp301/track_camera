#include "Capture.hpp"

Capture::Capture()
{
    capture = new cv::VideoCapture( 0 ); //set video device to default
}

Capture::~Capture()
{
    capture->release(); //disconnect camera before closing program
}

//grab current frame from input capture device
cv::Mat Capture::grabFrame()
{
    cv::Mat frame;
    capture->operator >>( frame ); //capture frame from video device

    return frame;
}

//check whether input device is ready
bool Capture::isReady()
{
    return ( capture->isOpened() ) ? true : false;
}
