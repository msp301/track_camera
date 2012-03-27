#include "Capture.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

Capture::Capture( cv::VideoCapture *camera, VideoBuffer *buffer )
{
    video_buffer = buffer;
    capture = camera; //store reference to camera device
}

Capture::~Capture()
{
    capture->release(); //disconnect camera before closing program
}

//thread implementation to start video stream capturing
void Capture::run()
{
    cout << "Capture: Started stream capture thread" << endl;
    while( true )
    {
        cv::Mat frame = grabFrame(); //grab frame from video device
        video_buffer->add( frame ); //add frame to video buffer
    }
}

//grab current frame from input capture device
cv::Mat Capture::grabFrame()
{
    cv::Mat frame;

    //check input device is available before reading
    if( isReady() )
    {
        cout << "Capture: About to grab frame" << endl;
        capture->operator >>( frame ); //capture frame from video device
    }

    return frame;
}

//check whether input device is ready
bool Capture::isReady()
{
    return ( capture->isOpened() ) ? true : false;
}


