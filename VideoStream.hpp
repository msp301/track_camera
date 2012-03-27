#ifndef VIDEOSTREAM_HPP
#define VIDEOSTREAM_HPP

#include <Capture.hpp>
#include <VideoBuffer.hpp>

class VideoStream
{
    public:
        VideoStream( VideoBuffer *video_buffer );
        void start();
        void stop();
        bool create();
        bool remove();
    private:
        Capture *capture;
        VideoBuffer *video_buffer;
};

#endif // VIDEOSTREAM_HPP
