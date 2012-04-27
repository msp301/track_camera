#ifndef CAPTURE_HPP
#define CAPTURE_HPP

#include "VideoBuffer.hpp"
#include "SafeThread.hpp"

#include <QThread>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class Capture : public QThread, public SafeThread
{
    public:
        Capture( cv::VideoCapture *camera, VideoBuffer *buffer );
        ~Capture();
        void run();
        cv::Mat grabFrame();
    public slots:
        void stopThread();
    private:
        cv::Mat frame;
        cv::VideoCapture *capture;
        VideoBuffer *video_buffer;
};

#endif // CAPTURE_HPP
