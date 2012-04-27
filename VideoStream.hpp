#ifndef VIDEOSTREAM_HPP
#define VIDEOSTREAM_HPP

#include <Capture.hpp>
#include <VideoBuffer.hpp>

#include <QObject>
#include <QTimer>

#include <opencv2/highgui/highgui.hpp>

class VideoStream : public QObject
{
    Q_OBJECT

    public:
        VideoStream( VideoBuffer *video_buffer );
        ~VideoStream();
        void start();
        void stop();
        bool create();
        bool remove();
    private:
        Capture *capture;
        VideoBuffer *video_buffer;
        cv::VideoCapture *camera;
        QTimer *timer_capture;
};

#endif // VIDEOSTREAM_HPP
