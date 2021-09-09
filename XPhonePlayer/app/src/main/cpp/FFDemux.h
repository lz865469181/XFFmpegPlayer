//
// Created by zhou.lu on 2021/9/1.
//

#ifndef XPHONEPLAYER_FFDEMUX_H
#define XPHONEPLAYER_FFDEMUX_H


#include "IDemux.h"
struct AVFormatContext;

class FFDemux : public IDemux {
public:
    //打开文件或者流媒体 rtmp/rtsp/http
    virtual bool Open(const char* url) ;
    virtual bool Seek(double pos);
    virtual void Close();

    //读取一帧数据，数据由调用者清理
    virtual XData Read();

    //获取视频参数
    virtual XParameter GetVPara();

    virtual XParameter GetAPara();

public:
    FFDemux();
    ~FFDemux();

private:
    AVFormatContext *ic = 0;
    int audioSteam = 1;
    int videoStream = 0;
    std::mutex mux;
};


#endif //XPHONEPLAYER_FFDEMUX_H
