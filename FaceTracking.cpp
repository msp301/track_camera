#include "FaceTracking.hpp"

#include <QDebug>
#include <QMutexLocker>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace std;

FaceTracking::FaceTracking( VideoBuffer *buffer, StandController *stand )
{
    video_buffer = buffer; //store local reference to given video buffer
    this->stand = stand; //store class accessible reference to stand controller
    haar_face_classifier_location =
            "libs/haar_cascades/haarcascade_frontalface_default.xml";

    haar_eye_classifier_location = "libs/haar_cascades/haarcascade_eye.xml";

    display_faces = false;
    mutex = new QMutex;
}

FaceTracking::~FaceTracking()
{
    this->quit(); //stop thread execution
    delete mutex;
}

void FaceTracking::run()
{
    exec(); //enter thread wait routine
}

//identify faces and determine their positions
void FaceTracking::trackFaces()
{
    vector<cv::Rect> faces;

    cv::Mat frame = video_buffer->read(); //read frame from video buffer

    //process only non-empty frames
    if( !frame.empty() )
    {
        faces = detectFace( frame );

        foreach( cv::Mat face, getFaceImages( frame, faces ) )
        {
            video_buffer->add( face );
        }

        /*if( faces.size() > 0 )
        {
            //receive and send position of the closest face to stand controller
            Coordinate track_position = getFacePosition( getClosestFace( faces ) );
            stand->sendFaceData( track_position );

            if( showDetectedFaces() ) displayDetectedFaces( frame, faces );
        }
        else
        {
            if( showDetectedFaces() ) displayDetectedFaces( frame, faces );
        }*/
    }
}

//detect a face within a given frame
vector<cv::Rect> FaceTracking::detectFace( cv::Mat frame )
{
    //create target frame size to resize received frame to before processing
    cv::Mat conv_frame = cv::Mat( ( frame.rows / 4 ),
                                  ( frame.cols / 4 ), CV_8UC2 );

    //scale image to reduce processing load
    cv::resize( frame, conv_frame, conv_frame.size(), 0, 0, CV_INTER_NN );

    cv::cvtColor( conv_frame, conv_frame, CV_BGR2GRAY ); //convert to greyscale
    cv::equalizeHist( conv_frame, conv_frame );

    vector<cv::Rect> faces = filterFaces( conv_frame );
    //vector<cv::Rect> eyes = filterEyes( conv_frame );

    //faces.insert( faces.end(), eyes.begin(), eyes.end() );

    //filterCorners( conv_frame ); //detect feature corners

    return faces; //return location of detected faces
}

vector<cv::Rect> FaceTracking::filterFaces( cv::Mat frame )
{
    cv::CascadeClassifier haar_face_classifier; //define classifier placeholder
    vector<cv::Rect> faces;

    //ensure we have received a valid video frame, before processing
    if( !frame.empty() )
    {
        //load face classifier
        if( haar_face_classifier.load( haar_face_classifier_location ) )
        {
            //match faces of different sizes within video frame
            haar_face_classifier.detectMultiScale( frame, faces, 1.1, 2,
                                    0 | CV_HAAR_SCALE_IMAGE, cv::Size( 2, 2 ) );

            qDebug() << "Faces detected = " << faces.size();
        }
    }

    return faces;
}

vector<cv::Rect> FaceTracking::filterEyes( cv::Mat frame )
{
    cv::CascadeClassifier haar_eye_classifier;
    vector<cv::Rect> eyes;

    //ensure we have received a valid video frame, before processing
    if( !frame.empty() )
    {
        if( haar_eye_classifier.load( haar_eye_classifier_location ) )
        {
            haar_eye_classifier.detectMultiScale( frame, eyes, 1.1, 2,
                                    0 | CV_HAAR_SCALE_IMAGE, cv::Size( 1, 1 ) );

            qDebug() << "Eyes detected = " << eyes.size();
        }
    }

    return eyes;
}

//detect and retrieve facial feature corners within given image
vector<cv::Point2f> FaceTracking::filterCorners( cv::Mat frame )
{
    vector<cv::Point2f> corners;
    cv::Mat conv_frame = cv::Mat( frame.rows, frame.cols, CV_8UC1 );

    cv::goodFeaturesToTrack( conv_frame, corners, 50, 0.01, 1 );
    cv::cornerSubPix( conv_frame, corners, cv::Size( 5, 5 ), cv::Size( -1, -1 ),
                     cv::TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001 ) );

    return corners;
}

//retrieve the largest face from a given list of detected faces
cv::Rect FaceTracking::getClosestFace( vector<cv::Rect> faces )
{
    cv::Rect largest_face;

    foreach( cv::Rect face, faces )
    {
        if( !largest_face.width && !largest_face.height )
        {
            //set largest face if this is the first face being compared
            largest_face = face;
        }
        else if( largest_face.area() < face.area() )
        {
            //update largest face if current is larger than previous
            largest_face = face;
        }
    }

    return largest_face;
}

//retrieve facial images
vector<cv::Mat> FaceTracking::getFaceImages( cv::Mat frame, vector<cv::Rect> faces )
{
    vector<cv::Mat> face_frames;

    foreach( cv::Rect face, faces )
    {
        //calculate face position, scaled from detected face coordinates
        cv::Rect roi = cv::Rect( ( face.x * 4 ), ( face.y * 4 ),
                                 ( face.width * 4 ), ( face.height * 4 ) );
        cv::Mat face_frame = frame( roi ); //extract face image

        face_frames.push_back( face_frame ); //add face to be returned
    }

    return face_frames;
}

//display detected faces to interface
void FaceTracking::displayDetectedFaces( cv::Mat frame, vector<cv::Rect> faces )
{
    vector<cv::Point2f> corners = filterCorners( frame );

    for( int i; i < corners.size(); i++ )
    {
        cv::circle( frame, corners[i], 4, cv::Scalar( 0, 255, 0, 0 ), -1, 8, 0 );
    }

    qDebug() << "displayDetectedFaces(): Displayed" << corners.size() <<
                "Corners";

    //loop through each detected face and draw a rectangle around it,
    //scaling back to original frame size
    foreach( cv::Rect face, faces )
    {
        cv::Point pt1( face.x * 4, face.y * 4 ); //top-left position of face area

        //calculate bottom-right coordinates of face given its area
        cv::Point pt2( ( ( ( ( face.x + face.width ) - 1 ) ) * 4 ),
                       ( ( ( ( face.y + face.height ) - 1 ) ) * 4 ) );

        //draw rectangle on image to specified size of face
        cv::rectangle( frame, pt1, pt2, cv::Scalar( 255, 0, 0, 0 ), 4, 8, 0 );
    }

    video_buffer->add( frame ); //add processed frame to output buffer
}

//retrieve position of a detected face
Coordinate FaceTracking::getFacePosition( cv::Rect face )
{
    Coordinate face_position;

    //find centre of face area
    face_position.x = ( ( face.x + face.width ) - 1 ) / 2;
    face_position.y = ( ( face.y + face.height ) - 1 ) / 2;

    return face_position;
}

//check whether faces should be displayed
bool FaceTracking::showDetectedFaces()
{
    QMutexLocker lock( mutex );
    return display_faces;
}

//set whether to display detected faces or not
void FaceTracking::toggleDisplayDetectedFaces()
{
    QMutexLocker lock( mutex );
    display_faces = ( display_faces ) ? false : true;
}
