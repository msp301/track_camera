#ifndef VIDEOBUFFER_HPP
#define VIDEOBUFFER_HPP

#include <vector>

#include <QString>

#include <opencv2/core/core.hpp>

using namespace std;

class VideoBuffer
{
    public:
        VideoBuffer( QString name );
        void add( cv::Mat frame );
        cv::Mat read();
        QString whoami();
    private:
        vector<cv::Mat> buffer;
        QString buffer_name;
};

#endif // VIDEOBUFFER_HPP
