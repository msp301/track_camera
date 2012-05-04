#include "TestCapture.hpp"

#include <QDebug>

TestCapture::TestCapture( cv::VideoCapture *camera, VideoBuffer *buffer ) :
    Capture( camera, buffer )
{
    capture = camera; //store reference to camera device
    video_buffer = buffer; //store reference to video buffer
}

//reimplemented video frame capture method
cv::Mat TestCapture::grabFrame()
{
    cv::Mat frame;

    //capture frame from video device, only if it exists
    //if( capture ) capture->operator >>( frame );

    //read image from file
    frame = cv::imread( "/home/martin/Pictures/Webcam/2012-05-03-174511.jpg" );
    usleep( 20 );

    qDebug() << "TestCapture: Reimplemented grabFrame()";

    return frame;
}
