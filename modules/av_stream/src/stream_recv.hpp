#ifndef __STREAM_RECV_HPP__
#define __STREAM_RECV_HPP__

#include "precomp.hpp"
#include <string>
#include <memory>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

class StreamRecv : public AVCapture {
public:
    StreamRecv() {}
    virtual ~StreamRecv();

    virtual bool open(std::string url);
    virtual bool grabFrame();
    virtual IplImage* retrieveFrame(int);

private:
    void cleanup();
    std::shared_ptr<AVPacket> ReadPacketFromSource();
    
    AVFormatContext *pFormatCtx;
    AVCodecContext  *pCodecCtx;
    AVStream        *video_stream;
    AVDictionary    *format_opts;
    AVCodec         *video_decoder;
    AVFrame         *pFrame;
    AVFrame          rgb_picture;
    AVPacket        *p_pkt;
    SwsContext      *img_convert_ctx;
    int             stream_index;
    
    float stream_fps;
    std::string stream_url;
    int pic_width;
    int pic_height;
    IplImage frame;
};

#endif // __STREAM_RECV_HPP__