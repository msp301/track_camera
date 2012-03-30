#ifndef DISPLAYSTREAM_HPP
#define DISPLAYSTREAM_HPP

#include "VideoBuffer.hpp"

#include <QThread>
#include <QMutex>

#include <opencv2/core/core.hpp>

class DisplayStream : public QThread
{
    Q_OBJECT

    public:
        DisplayStream( VideoBuffer *buffer );
        void run();
        void setVideoBuffer( VideoBuffer *buffer );
    signals:
        void frameReady( cv::Mat frame );
    private:
        VideoBuffer *video_buffer;
        QMutex *mutex;
};

#endif // DISPLAYSTREAM_HPP
