#ifndef FACETRACKING_HPP
#define FACETRACKING_HPP

#include "Capture.hpp"
#include "VideoBuffer.hpp"

#include <string>

#include <QThread>

#include <opencv2/core/core.hpp>

class FaceTracking : public QThread
{
    Q_OBJECT

    public:
        FaceTracking( VideoBuffer *buffer );
        void run();
    signals:
        void frameReady( cv::Mat frame );
    private:
        vector<cv::Rect> detectFace( cv::Mat frame );
        void displayDetectedFaces( cv::Mat frame, vector<cv::Rect> faces );

        VideoBuffer *video_buffer;
        string haar_face_classifier_location;
};

#endif // FACETRACKING_HPP
