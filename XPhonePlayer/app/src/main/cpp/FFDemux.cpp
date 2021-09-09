//
// Created by zhou.lu on 2021/9/1.
//

#include "FFDemux.h"
#include "XLog.h"
extern "C"
{
#include <libavformat/avformat.h>
}
//分数转为浮点数
static double r2d(AVRational r)
{
    return r.num == 0 || r.den == 0 ? 0.0 : (double )r.num/(double )r.den;
}

bool FFDemux::Open(const char *url) {
    XLOGI("Open file %s begin", url);
    Close();
    mux.lock();
    int ret = avformat_open_input(&ic, url, NULL, NULL);
    if(ret != 0)
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        XLOGE("FFDemux open %s Failed! %s", url, buf);
        return false;
    }
    XLOGI("FFDemux open %s success!",url);

    //读取文件信息
    ret = avformat_find_stream_info(ic, NULL);
    if(ret != 0)
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        XLOGE("FFDemux avformat_find_stream_info %s Failed! %s", url, buf);
        return false;
    }
    this->totalMs = ic->duration/(AV_TIME_BASE/1000);
    XLOGI("total ms = %d", totalMs);
    mux.unlock();
    //获取流索引
    GetVPara();
    GetAPara();
    return true;
}

XData FFDemux::Read() {
    mux.lock();
    if (!ic) {
        mux.lock();
        return XData();
    }
    XData d;
    AVPacket* pkt = av_packet_alloc();
    int ret = av_read_frame(ic, pkt);
    if(ret != 0)
    {
        mux.unlock();
        av_packet_free(&pkt);
        return XData();
    }
    //XLOGI("packet size  is %d ptss %lld", pkt->size, pkt->pts);
    d.data = (unsigned char *)pkt;
    d.size = pkt->size;
    if(pkt->stream_index == audioSteam)
    {
        d.isAudio = true;
    }
    else if(pkt->stream_index == videoStream)
    {
        d.isAudio = false;
    }
    else
    {
        mux.unlock();
        av_packet_free(&pkt);
        return XData();
    }
    //转换pts
    pkt->pts =pkt->pts * (1000 * r2d(ic->streams[pkt->stream_index]->time_base));
    pkt->dts =pkt->dts * (1000 * r2d(ic->streams[pkt->stream_index]->time_base));
    d.pts = (int)pkt->dts;
    mux.unlock();
    return d;
}

void FFDemux::Close() {
    mux.lock();
    if(ic)
        avformat_close_input(&ic);
    mux.unlock();
}

FFDemux::FFDemux()
{
    static bool isFirst = true;
    if(isFirst)
    {
        //注册所有封装器
        av_register_all();

        //注册解码器
        avcodec_register_all();

        //初始化网络
        avformat_network_init();

        XLOGI("register ffmpeg");
        //非线程安全
        isFirst = false;
    }
    ic = NULL;
};

FFDemux::~FFDemux() {

}

XParameter FFDemux::GetVPara() {
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        XLOGE("GetVPara failed!ic is NULL！");
        return XParameter();
    }


    //获取视频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if(re < 0)
    {
        mux.unlock();
        XLOGE("av_find_best_stream failed");
        return XParameter();
    }

    videoStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    mux.unlock();
    return para;
}

XParameter FFDemux::GetAPara() {
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        XLOGE("GetAPara failed! ic is NULL！");
        return XParameter();
    }

    //获取音频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if(re < 0)
    {
        mux.unlock();
        XLOGE("av_find_best_stream failed");
        return XParameter();
    }

    audioSteam = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    para.channels = ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;
    mux.unlock();
    return para;
}

bool FFDemux::Seek(double pos) {
    if(pos<0 || pos > 1)
    {
        XLOGE("Seek value must 0.0~1.0");
        return false;
    }
    bool re = false;
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        return false;
    }
    //清理读取的缓冲
    avformat_flush(ic);
    long long seekPts = 0;
    seekPts = ic->streams[videoStream]->duration*pos;

    //往后跳转到关键帧
    re = av_seek_frame(ic,videoStream,seekPts,AVSEEK_FLAG_FRAME|AVSEEK_FLAG_BACKWARD);
    mux.unlock();
    return re;
}
