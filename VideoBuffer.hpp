#ifndef VIDEOBUFFER_HPP
#define VIDEOBUFFER_HPP

#include <QMutex>

#include <opencv2/core/core.hpp>

using namespace std;

class VideoBuffer
{
    public:
        VideoBuffer();
        void add( cv::Mat frame );
        cv::Mat read();
    private:
        cv::Mat *buffer;
        int buffer_head;
        int buffer_tail;
        int buffer_size;
        QMutex *mutex;
};

#endif // VIDEOBUFFER_HPP
