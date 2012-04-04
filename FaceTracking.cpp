#include "FaceTracking.hpp"

#include <QDebug>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace std;

struct FaceTracking::coordinate
{
    int x, y;
};

FaceTracking::FaceTracking( VideoBuffer *buffer, VideoBuffer *output )
{
    video_buffer = buffer; //store local reference to given video buffer
    output_buffer = output; //store reference to given output video buffer
    haar_face_classifier_location =
            "/home/martin/src/OpenCV-2.3.1/data/haarcascades/haarcascade_frontalface_default.xml";
}

void FaceTracking::run()
{
    vector<cv::Rect> faces;

    //class thread implementation
    while( true )
    {
        msleep( 60 );
        cv::Mat frame = video_buffer->read(); //read frame from video buffer

        //process only non-empty frames
        if( !frame.empty() )
        {
            faces = detectFace( frame );

            if( faces.size() > 0 )
            {
                qDebug() << "Frame Accepted";
                getFacePositions( faces );
                displayDetectedFaces( frame, faces );
            }
            else
            {
                qDebug() << "Frame Discarded";
            }
        }
    }
}

//detect a face within a given frame//process non-empty frames only
vector<cv::Rect> FaceTracking::detectFace( cv::Mat frame )
{
    cv::Mat conv_frame;
    cv::CascadeClassifier haar_face_classifier;
    vector<cv::Rect> faces;

    cv::cvtColor( frame, conv_frame, CV_BGR2GRAY ); //convert to greyscale
    cv::equalizeHist( conv_frame, conv_frame );

    //load face classifier
    if( haar_face_classifier.load( haar_face_classifier_location ) )
    {
        //match faces of different sizes within video frame
        haar_face_classifier.detectMultiScale( conv_frame, faces, 1.1, 2,
                                0 | CV_HAAR_SCALE_IMAGE, cv::Size( 30, 30 ) );

        qDebug() << "Faces detected = " << faces.size();
    }

    return faces; //return location of detected faces
}

//display detected faces to interface
void FaceTracking::displayDetectedFaces( cv::Mat frame, vector<cv::Rect> faces )
{
    //loop through each detected face and draw a rectangle around it
    foreach( cv::Rect face, faces )
    {
        cv::Point pt1( face.x, face.y ); //top-left position of face area

        //calculate bottom-right coordinates of face given its area
        cv::Point pt2( ( ( face.x + face.width ) - 1 ),
                       ( ( face.y + face.height ) - 1 ) );

        //draw rectangle on image to specified size of face
        cv::rectangle( frame, pt1, pt2, cv::Scalar( 255, 0, 0, 0 ), 4, 8, 0 );

        //emit( frameReady( frame ) ); //inform UI of new frame
        output_buffer->add( frame ); //add processed frame to output buffer
    }
}

//determine central positions for each identified face
vector<FaceTracking::coordinate> FaceTracking::getFacePositions(
        vector<cv::Rect> faces )
{
    vector<FaceTracking::coordinate> coordinates;
    FaceTracking::coordinate face_position;

    //find central position of detected face areas
    foreach( cv::Rect face, faces )
    {
        //find centre of given face area
        face_position.x = ( ( face.x + face.width ) - 1 ) / 2;
        face_position.y = ( ( face.y + face.height ) - 1 ) / 2;

        //add coordinate to result vector
        coordinates.push_back( face_position );
    }

    return coordinates;
}
