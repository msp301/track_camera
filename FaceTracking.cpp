#include "FaceTracking.hpp"

#include <QDebug>
#include <QMutexLocker>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/video/tracking.hpp>

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
    vector<cv::RotatedRect> prob_faces;

    cv::Mat frame = video_buffer->read(); //read frame from video buffer

    //process only non-empty frames
    if( !frame.empty() )
    {
        faces = detectFace( frame );

        if( faces.size() > 0 )
        {
            prev_faces.clear();
            foreach( cv::Rect face, faces )
            {
                prev_faces.push_back( face ); //update last seen face
            }
        }

        cv::RotatedRect face_area;
        if( faces.size() > 0 )
        {
            qDebug() << "Frontal Faces = " << faces.size();
            setTrackFace( frame, faces.at( 0 ) );
            //position = cv::Point2f( ( face_area.center.x * 4 ),
            //                        ( face_area.center.y * 4 ) );
            //cv::circle( frame, position, 4, cv::Scalar( 0, 255, 0, 0 ),
            //            -1, 8, 0 );
        }
        else
        {
            qDebug() << "No Frontal Faces";

            prob_faces.clear();
            foreach( cv::Rect face, prev_faces )
            {
                face_area = trackFace( frame, face );
                prob_faces.push_back( face_area );
            }

            //cv::circle( frame, position, 4, cv::Scalar( 0, 255, 0, 0 ),
            //            -1, 8, 0 );
        }

        /*foreach( cv::Mat face, getFaceImages( frame, faces ) )
        {
            vector<cv::Point2f> corners = filterCorners( face );

            for( int i; i < corners.size(); i++ )
            {
                cv::circle( face, corners[i], 4, cv::Scalar( 0, 255, 0, 0 ), -1, 8, 0 );
            }

            video_buffer->add( face );
        }*/

        if( faces.size() > 0 )
        {
            //receive and send position of the closest face to stand controller
            Coordinate track_position = getFacePosition( getClosestFace( faces ) );
            stand->sendFaceData( track_position );

            qDebug() << "Actual face location at: " << track_position.x
                     << "," << track_position.y;

            if( showDetectedFaces() ) displayDetectedFaces( frame, faces );
        }
        else if( prob_faces.size() > 0 )
        {
            //recieve and send estimated position of face to stand controller
            Coordinate track_position = getFacePosition( prob_faces.at( 0 ) );
            stand->sendFaceData( track_position );

            qDebug() << "Estimated face location at: " << track_position.x
                     << "," << track_position.y;

            if( showDetectedFaces() ) displayDetectedFaces( frame, prob_faces );
        }
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

//set hue image to use for locating lost faces
cv::Mat FaceTracking::convHueImage( cv::Mat image )
{
    int smin = 50;
    int vmin = 60;
    int vmax = 150;

    qDebug() << "Converting colour";
    cv::cvtColor( image, image, CV_BGR2HSV ); //convert to HSV format

    qDebug() << "Setting range";
    cv::inRange( image, cv::Scalar( 0, smin, vmin, 0 ),
                 cv::Scalar( 180, 256, vmax, 0 ), mask );

    qDebug() << "Splitting channels";
    vector<cv::Mat> colour_channels;
    cv::split( image, colour_channels ); //separate colour channels

    qDebug() << "Exiting convHueImage()";
    return colour_channels.at( 0 ); //extract hue channel from HSV image
}

//track a given face
void FaceTracking::setTrackFace( cv::Mat frame, cv::Rect face )
{
    cv::Mat face_image;

    //create target frame size to resize received frame to before processing
    cv::Mat conv_face_image = cv::Mat( ( frame.rows / 4 ),
                                  ( frame.cols / 4 ), frame.type() );

    getFaceImage( frame, face ).copyTo( face_image );

    //scale image to reduce processing load
    cv::resize( face_image, conv_face_image,
                conv_face_image.size(), 0, 0, CV_INTER_NN );

    cv::Mat hue_face_image = cv::Mat( conv_face_image.rows,
                                      conv_face_image.cols, CV_8UC2 );

    qDebug() << "About to conv hue";
    convHueImage( conv_face_image ).copyTo( hue_face_image ); //convert to hue image
    qDebug() << "Copied hue face image";

    int hbins = 30, sbins = 32;
    int histSize[] = { hbins, sbins };
    float hranges[] = { 0, 180 };
    float sranges[] = { 0, 256 };
    const float *ranges[] = { hranges, sranges };
    int channels[] = { 0, 1 };

    qDebug() << "About to calculate Hist";
    cv::calcHist( &frame, 1, channels, mask, hist, 2, histSize,
                  ranges, true, false );
    qDebug() << "Calculated Hist";
    cv::normalize( hist, hist, 0, 255, CV_MINMAX );
    qDebug() << "Normalised Hist";

    double histMax;
    cv::minMaxLoc( hist, 0, &histMax );
    hist *= histMax ? 255.0 / histMax : 0.0;
}

//locate face that has been lost
cv::RotatedRect FaceTracking::trackFace( cv::Mat frame, cv::Rect prev_face )
{
    qDebug() << "Entered trackFace()";

    //cv::Mat hue_frame = cv::Mat( frame.rows, frame.cols, frame.type() );
    //convHueImage( frame ).copyTo( hue_frame );

    float hranges[] = { 0, 180 };
    float sranges[] = { 0, 256 };
    const float *ranges[] = { hranges, sranges };
    int channels[] = { 0, 1 };
    cv::Mat bp;

    cv::calcBackProject( &frame, 1, channels, hist, bp, ranges, 1, true );
    //cv::bitwise_and( bp, mask, bp, 0 );

    cv::RotatedRect foundObject = cv::CamShift( bp, prev_face,
       cv::TermCriteria( cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 10, 1 ) );

    return foundObject;
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

    cv::goodFeaturesToTrack( conv_frame, corners, 50, 0.01, 10 );
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

//retrieve facial image for a single face
cv::Mat FaceTracking::getFaceImage( cv::Mat frame, cv::Rect face )
{
    //calculate face position, scaled from detected face coordinates
    cv::Rect roi = cv::Rect( ( face.x * 4 ), ( face.y * 4 ),
                             ( face.width * 4 ), ( face.height * 4 ) );
    cv::Mat face_frame = frame( roi ); //extract face image

    return face_frame;
}

//display detected faces to interface
void FaceTracking::displayDetectedFaces( cv::Mat frame, vector<cv::Rect> faces )
{
    /*vector<cv::Point2f> corners = filterCorners( frame );

    for( int i; i < corners.size(); i++ )
    {
        cv::circle( frame, corners[i], 4, cv::Scalar( 0, 255, 0, 0 ), -1, 8, 0 );
    }

    qDebug() << "displayDetectedFaces(): Displayed" << corners.size() <<
                "Corners";
    */

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

//display detected rotated faces to interface
void FaceTracking::displayDetectedFaces( cv::Mat frame,
                                         vector<cv::RotatedRect> faces )
{
    //loop through each detected face and draw a rectangle around its
    //area boundaries
    foreach( cv::RotatedRect face, faces )
    {
        cv::Rect face_area = face.boundingRect(); //get face area boundaries

        //top-left position of face area
        cv::Point pt1( face_area.x * 4, face_area.y * 4 );

        //calculate bottom-right coordinates of face given its area
        cv::Point pt2( ( ( ( ( face_area.x + face_area.width ) - 1 ) ) * 4 ),
                       ( ( ( ( face_area.y + face_area.height ) - 1 ) ) * 4 ) );

        cv::rectangle( frame, pt1, pt2, cv::Scalar( 0, 255, 0, 0 ), 4, 8, 0 );
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

//retrieve position of detected rotated face
Coordinate FaceTracking::getFacePosition( cv::RotatedRect face )
{
    Coordinate face_position;

    //find centre of rotated face area
    face_position.x = face.center.x;
    face_position.y = face.center.y;

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
