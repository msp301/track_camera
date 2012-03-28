#ifndef VIDEOBUFFER_HPP
#define VIDEOBUFFER_HPP

#include <vector>

#include <opencv2/core/core.hpp>

using namespace std;

class VideoBuffer
{
    public:
        VideoBuffer();
        void add( cv::Mat frame );
        cv::Mat read();
    private:
        vector<cv::Mat> buffer;
};

#endif // VIDEOBUFFER_HPP
