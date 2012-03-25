#ifndef CAPTURE_HPP
#define CAPTURE_HPP

#include <QImage>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class Capture
{
    public:
        Capture();
        ~Capture();
        cv::Mat grabFrame();
        QImage grabConvertedFrame();
        cv::Mat grabStream();
        bool isReady();
    private:
        cv::Mat frame;
        cv::VideoCapture *capture;
};

#endif // CAPTURE_HPP
