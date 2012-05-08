#ifndef FACETRACKING_HPP
#define FACETRACKING_HPP

#include "Capture.hpp"
#include "VideoBuffer.hpp"
#include "StandController.hpp"
#include "Coordinate.hpp"

#include <string>

#include <QThread>
#include <QMutex>

#include <opencv2/core/core.hpp>

class FaceTracking : public QThread
{
    Q_OBJECT

    public:
        FaceTracking( VideoBuffer *buffer, StandController *stand );
        ~FaceTracking();
        void run();
        void toggleDisplayDetectedFaces();
        bool showDetectedFaces();
    public slots:
        void trackFaces();
    private:
        VideoBuffer *video_buffer;
        StandController *stand;
        string haar_face_classifier_location;
        vector<cv::Rect> previous_detected_faces;
        bool display_faces;
        QMutex *mutex;

        vector<cv::Rect> detectFace( cv::Mat frame );
        cv::Rect getClosestFace( vector<cv::Rect> faces );
        void displayDetectedFaces( cv::Mat frame, vector<cv::Rect> faces );
        Coordinate getFacePosition( cv::Rect face );
};

#endif // FACETRACKING_HPP
