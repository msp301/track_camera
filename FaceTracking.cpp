#include "FaceTracking.hpp"

#include <iostream>

using namespace std;

FaceTracking::FaceTracking( Capture *camera )
{
    input = camera; //store local reference to given input device
}

void FaceTracking::run()
{
    //class thread implementation
    while( true )
    {
        cv::Mat frame = input->grabFrame();
        detectFace( frame );
    }
}

//detect a face within a given frame
void FaceTracking::detectFace( cv::Mat frame )
{
}
