#ifndef FACETRACKING_HPP
#define FACETRACKING_HPP

#include "Capture.hpp"

#include <QThread>

#include <opencv2/core/core.hpp>

class FaceTracking : public QThread
{
    public:
        FaceTracking( Capture *camera );
        void run();
    private:
        void detectFace( cv::Mat frame );

        Capture *input;
};

#endif // FACETRACKING_HPP
