#include "VideoBuffer.hpp"

#include <QDebug>

VideoBuffer::VideoBuffer( QString name )
{
    buffer_name = name;
}

//insert frame onto end of buffer
void VideoBuffer::add( cv::Mat frame )
{
    if( buffer_name == "tracking" ) qDebug() << "Adding frame to " << buffer_name;
    buffer.push_back( frame ); //push frame onto end of buffer
    //qDebug() << "Buffer " << buffer_name << "now = " << buffer.size();
}

//access frame from front of buffer
cv::Mat VideoBuffer::read()
{
    cv::Mat frame;

    //ensure data exists in the buffer before reading
    if( buffer.size() > 0 )
    {
        //qDebug() << "Reading from: " << buffer_name;
        frame = buffer.front(); //return frame on the front of the buffer
        //buffer.erase( buffer.begin() );
    }

    return frame;
}

QString VideoBuffer::whoami()
{
    return buffer_name;
}
