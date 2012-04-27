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
    //check thread has not been scheduled to be stopped
    while( !isStopped() )
    {
        cv::Mat frame = grabFrame(); //grab frame from video device

        //do not write empty frames to video buffer
        if( !frame.empty() )
        {
            video_buffer->add( frame ); //add frame to video buffer
        }

        msleep( 60 );
    }

    qDebug() << "Capture: About to enter exec()";
    exec(); //enter thread wait routine
}

//grab current frame from input capture device
cv::Mat Capture::grabFrame()
{
    cv::Mat frame;

    capture->operator >>( frame ); //capture frame from video device

    return frame;
}

