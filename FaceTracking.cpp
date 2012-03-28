#include "FaceTracking.hpp"

#include <QDebug>

using namespace std;

FaceTracking::FaceTracking( VideoBuffer *buffer )
{
    video_buffer = buffer; //store local reference to given video buffer
}

void FaceTracking::run()
{
    //class thread implementation
    while( true )
    {
        cv::Mat frame = video_buffer->read(); //read frame from video buffer
        detectFace( frame );
    }
}

//detect a face within a given frame
void FaceTracking::detectFace( cv::Mat frame )
{
    qDebug() << "detectFace: Should do some image processing here";
}
