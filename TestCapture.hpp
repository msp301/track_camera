#ifndef TESTCAPTURE_HPP
#define TESTCAPTURE_HPP

#include "Capture.hpp"

class TestCapture : public Capture
{
    Q_OBJECT

    public:
        TestCapture( cv::VideoCapture *camera, VideoBuffer *buffer );
        cv::Mat grabFrame();
    private:
        VideoBuffer *video_buffer;
        cv::VideoCapture *capture;
};

#endif // TESTCAPTURE_HPP
