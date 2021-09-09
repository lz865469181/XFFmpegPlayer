//
// Created by zhou.lu on 2021/9/8.
//

#ifndef XPHONEPLAYER_IPLAYER_H
#define XPHONEPLAYER_IPLAYER_H


#include "XThread.h"
#include "XParameter.h"
#include <mutex>
class IDemux;
class IDecode;
class IResample;
class IVideoView;
class IAudioPlayer;
class IPlayer : public XThread{
public:
    static IPlayer *Get(unsigned char index = 0);
    virtual bool Open(const char* path);
    virtual void Close();
    virtual bool Start();

    virtual void InitView(void* win);

    //获取当前的播放进度 0.0 ~ 1.0
    virtual double PlayPos();
    virtual bool Seek(double pos);
    virtual void SetPause(bool isP);

    virtual void SetHardDecode(bool bHard);

public:
    //音频输出参数配置
    XParameter outParam;
    //用户设定是否硬解码
    bool isHardDecode = true;
    IDemux* demux = 0;
    IDecode *vdecode = 0;
    IDecode *adecode = 0;
    IResample *resample = 0;
    IVideoView *videoView = 0;
    IAudioPlayer *audioPlayer = 0;

protected:
    //用作音视频同步
    void Main();
    std::mutex mux;
    IPlayer();
};


#endif //XPHONEPLAYER_IPLAYER_H
