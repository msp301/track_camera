#ifndef VIDEOBUFFER_HPP
#define VIDEOBUFFER_HPP

#include <QMutex>

#include <QString>

#include <opencv2/core/core.hpp>

using namespace std;

class VideoBuffer
{
    public:
        VideoBuffer( QString name );
        ~VideoBuffer();
        void add( cv::Mat frame );
        cv::Mat read();
        QString whoami();
    private:
        cv::Mat *buffer;
        int buffer_head;
        int buffer_tail;
        int buffer_size;
        QMutex *mutex;
        QString buffer_name;
};

#endif // VIDEOBUFFER_HPP
