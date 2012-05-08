#include "TestFilters.hpp"

#include <QDebug>

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

        cv::blur( conv_frame, conv_frame, cv::Size( 3, 3 ) );
        cv::Laplacian( conv_frame, result, CV_DEPTH_MAX, 3 );

        qDebug() << "Frame Type = " << frame.type();
        qDebug() << "Conv Frame Type = " << conv_frame.type();
    }

    return result;
}

cv::Mat TestFilters::diffOfGaussians( cv::Mat frame )
{
    cv::Mat *result_1 = new cv::Mat( frame.rows, frame.cols, frame.type() );
    cv::Mat *result_2 = new cv::Mat( frame.rows, frame.cols, frame.type() );

    cv::GaussianBlur( frame, result_1, cv::Size( 3, 3 ), 1.4 );
    cv::GaussianBlur( frame, result_1, cv::Size( 3, 3 ), 3 );


    //cv::Mat kernel_x = cv::getGaussianKernel( 3, 1, CV_32F );
    //cv::sepFilter2D( frame, result_1, frame.depth(), kernel_x,  )
}
