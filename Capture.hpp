#ifndef CAPTURE_HPP
#define CAPTURE_HPP

#include "VideoBuffer.hpp"

#include <QThread>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class Capture : public QThread
{
    Q_OBJECT

    public:
        Capture( cv::VideoCapture *camera, VideoBuffer *buffer );
        ~Capture();
        virtual void run();
        virtual cv::Mat grabFrame();
    public slots:
        virtual void captureFrame();
    private:
        cv::Mat frame;
        cv::VideoCapture *capture;
        VideoBuffer *video_buffer;
};

#endif // CAPTURE_HPP
