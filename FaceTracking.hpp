#ifndef FACETRACKING_HPP
#define FACETRACKING_HPP

#include "Capture.hpp"
#include "VideoBuffer.hpp"

#include <QThread>

#include <opencv2/core/core.hpp>

class FaceTracking : public QThread
{
    public:
        FaceTracking( VideoBuffer *buffer );
        void run();
    private:
        void detectFace( cv::Mat frame );

        VideoBuffer *video_buffer;
};

#endif // FACETRACKING_HPP
