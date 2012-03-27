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

//grab current frame and convert to QImage format
QImage Capture::grabConvertedFrame()
{
    cv::Mat frame = grabFrame(); //grab current frame
    cv::Mat conv_frame; //initialise converted frame

    cv::cvtColor( frame, conv_frame, CV_BGR2RGB ); //convert image to use RBG colour
    QImage image( (uchar*) frame.data, frame.cols,
                  frame.rows, QImage::Format_RGB32 );

    return image;
}

//check whether input device is ready
bool Capture::isReady()
{
    return ( capture->isOpened() ) ? true : false;
}

//start a video stream, grabbing each frame in turn
/*cv::Mat Capture::grabStream()
{
    //check if input device is ready before starting
    if( isReady() )
    {
        //constantly sample video input
        for( ;; )
        {
            cv::Mat frame;
            frame = grabFrame(); //read next frame from camera
        }
    }
}*/

