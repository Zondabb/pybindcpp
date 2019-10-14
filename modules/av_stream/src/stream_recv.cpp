#include <sstream>

#include "config.hpp"
#include "stream_recv.hpp"

StreamRecv::~StreamRecv() {
    cleanup();
}

void StreamRecv::cleanup() {
    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);
    av_dict_free(&format_opts);
    sws_freeContext(img_convert_ctx);
}

bool StreamRecv::open(std::string url) {
    int ret, got_picture;

    pFormatCtx = NULL;
    pCodecCtx = NULL;
    video_stream = NULL;
    format_opts = NULL;
    video_decoder = NULL;
    memset( &rgb_picture, 0, sizeof(rgb_picture) );

    stream_url = url;
    av_register_all();
    avformat_network_init();
    av_log_set_level(AV_LOG_FATAL);
    av_log_set_flags(AV_LOG_PRINT_LEVEL);

reopen_rtsp:
    if (!av_dict_get(format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE)) {
        av_dict_set(&format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
    }
    av_dict_set_int(&format_opts, "stimeout", 5 * 1000 * 1000, 0); // 5 second
    av_dict_set(&format_opts, "rtsp_transport", "tcp", 0);
    do {
        INFO_LOG("opening a connection.");
        ret = avformat_open_input(&pFormatCtx, stream_url.c_str(), NULL, &format_opts);
        if (ret < 0) {
            INFO_LOG("open connection %s failed, will try open later.", stream_url.c_str());
            return false;
        } else {
            INFO_LOG("open connection %s success.", stream_url.c_str());
            break;
        }
    } while(1);

    ret = avformat_find_stream_info(pFormatCtx, NULL);
    if (ret < 0) {
        INFO_LOG("call avformat_find_stream_info failed");
        return false;
    }

    stream_index = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &video_decoder, 0);
    if (stream_index < 0) {
        INFO_LOG("find video stream failed");
        return false;
    }

    av_dump_format(pFormatCtx, stream_index, stream_url.c_str(), 0);
    // video
    video_stream = pFormatCtx->streams[stream_index];
    pCodecCtx = video_stream->codec;
    video_decoder = avcodec_find_decoder(pCodecCtx->codec_id);
    if(video_decoder == NULL){
        INFO_LOG("Codec not found");
        return false;
    }
    if(avcodec_open2(pCodecCtx, video_decoder, NULL)<0){
        INFO_LOG("Could not open codec");
        return false;
    }

    stream_fps = av_q2d(video_stream->avg_frame_rate);
    if (stream_fps == 0.0f) {
        stream_fps = av_q2d(video_stream->r_frame_rate);
    }
    
    if (stream_fps == 0.0f) {
        INFO_LOG("unable to get fps, use fps = 25");
        stream_fps = 25.0f;
    } else {
        INFO_LOG("get fps = %f", stream_fps);
    }

    img_convert_ctx = sws_getContext(
        pCodecCtx->width, pCodecCtx->height, 
        pCodecCtx->pix_fmt,
        pCodecCtx->width, pCodecCtx->height, 
        AV_PIX_FMT_BGR24, 
        SWS_BICUBIC, 
        NULL, NULL, NULL);

    pFrame = av_frame_alloc();
    pic_width = video_stream->codecpar->width;
    pic_height = video_stream->codecpar->height;
    INFO_LOG("picture w = %d, h = %d", pic_width, pic_height);

    int buffer_width = pic_width;
    int buffer_height = pic_height;
    int aligns[AV_NUM_DATA_POINTERS];
    avcodec_align_dimensions2(video_stream->codec, &buffer_width, &buffer_height, aligns);
    INFO_LOG("buffer w = %d, h = %d", buffer_width, buffer_height);
    rgb_picture.data[0] = (uint8_t*)realloc(rgb_picture.data[0],
        av_image_get_buffer_size(AV_PIX_FMT_BGR24,
            buffer_width,
            buffer_height,
            1));
    av_image_fill_arrays(
        rgb_picture.data,
        rgb_picture.linesize,
        rgb_picture.data[0],
        AV_PIX_FMT_BGR24,
        buffer_width,
        buffer_height, 1);
    return true;
}

bool StreamRecv::grabFrame() {
    bool valid = false;
    auto pkt = ReadPacketFromSource();
    if (!pkt) {
        return false;
    }
    p_pkt = pkt.get();
    if (pkt.get()->stream_index == stream_index) {
        int got_picture = 0;
        avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, pkt.get());
        if(got_picture) {
            valid = true;
        } else {
            valid = false;
        }
    }
    return valid;
}

IplImage* StreamRecv::retrieveFrame(int) {
    sws_scale(
        img_convert_ctx,
        pFrame->data,
        pFrame->linesize,
        0, pCodecCtx->height,
        rgb_picture.data,
        rgb_picture.linesize
        );
    cvInitImageHeader(&frame, cvSize(pic_width, pic_height), 8, 3);
    cvSetData(&frame, rgb_picture.data[0], rgb_picture.linesize[0]);
    return &frame;
}

std::shared_ptr<AVPacket> StreamRecv::ReadPacketFromSource() {
    std::shared_ptr<AVPacket> packet(static_cast<AVPacket*>(av_malloc(sizeof(AVPacket))), [&](AVPacket *p) { av_free_packet(p); av_freep(&p);});
    av_init_packet(packet.get());
    int ret = av_read_frame(pFormatCtx, packet.get());
    if(ret >= 0) {
        return packet;
    } else {
        INFO_LOG("av_read_frame failed, exit!");
        exit(0);
        // return NULL;
    }
}