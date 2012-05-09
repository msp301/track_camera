#include "TestFilters.hpp"

#include <vector>

#include <QDebug>

#include <opencv2/nonfree/features2d.hpp>

using namespace std;

TestFilters::TestFilters()
{
}

cv::Mat TestFilters::laplacianOfGaussian( cv::Mat frame )
{
    cv::Mat result;
    cv::Mat conv_frame;

    if( !frame.empty() )
    {
        //cv::cvtColor( frame, conv_frame, CV_BGR2GRAY ); //convert to greyscale
        conv_frame = frame.clone();

        //cv::blur( conv_frame, conv_frame, cv::Size( 3, 3 ) );
        cv::GaussianBlur( conv_frame, conv_frame, cv::Size( 3, 3 ), 0, 0 );
        cv::Laplacian( conv_frame, result, CV_DEPTH_MAX, 3 );
        cv::convertScaleAbs( result, result );

        qDebug() << "Frame Type = " << frame.type();
        qDebug() << "Conv Frame Type = " << conv_frame.type();
    }

    return result;
}

cv::Mat TestFilters::diffOfGaussians( cv::Mat frame )
{
    cv::Mat *result_1 = new cv::Mat( frame.rows, frame.cols, frame.type() );
    cv::Mat *result_2 = new cv::Mat( frame.rows, frame.cols, frame.type() );

    //cv::GaussianBlur( frame, result_1, cv::Size( 3, 3 ), 1.4 );
    //cv::GaussianBlur( frame, result_1, cv::Size( 3, 3 ), 3 );


    //cv::Mat kernel_x = cv::getGaussianKernel( 3, 1, CV_32F );
    //cv::sepFilter2D( frame, result_1, frame.depth(), kernel_x,  )
}

cv::Mat TestFilters::surf( cv::Mat frame )
{
    cv::Mat image;
    //cv::SURF *extractor = new cv::SURF;
    //vector<cv::KeyPoint> points;

    //cv::cvtColor( frame, image, CV_BGR2GRAY ); //convert to greyscale
    //extractor->detect( image, points );
}
