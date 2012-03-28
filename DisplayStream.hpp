#ifndef DISPLAYSTREAM_HPP
#define DISPLAYSTREAM_HPP

#include "VideoBuffer.hpp"

#include <QThread>

#include <opencv2/core/core.hpp>

class DisplayStream : public QThread
{
    Q_OBJECT

    public:
        DisplayStream( VideoBuffer *buffer );
        void run();
    signals:
        void frameReady( cv::Mat frame );
    private:
        VideoBuffer *video_buffer;
};

#endif // DISPLAYSTREAM_HPP
