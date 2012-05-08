#ifndef TESTFILTERS_HPP
#define TESTFILTERS_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class TestFilters
{
    public:
        TestFilters();
        cv::Mat laplacianOfGaussian( cv::Mat frame );
        cv::Mat diffOfGaussians( cv::Mat frame );
};

#endif // TESTFILTERS_HPP
