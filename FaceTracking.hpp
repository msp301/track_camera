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
        FaceTracking( VideoBuffer *buffer, VideoBuffer *output );
        void run();
    signals:
        //void frameReady( cv::Mat frame );
    private:
        VideoBuffer *video_buffer;
        VideoBuffer *output_buffer;
        string haar_face_classifier_location;
        struct coordinate;
        vector<cv::Rect> previous_detected_faces;

        vector<cv::Rect> detectFace( cv::Mat frame );
        void displayDetectedFaces( cv::Mat frame, vector<cv::Rect> faces );
        vector<coordinate> getFacePositions( vector<cv::Rect> faces );
};

#endif // FACETRACKING_HPP
