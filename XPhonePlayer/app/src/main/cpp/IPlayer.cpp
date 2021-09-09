//
// Created by zhou.lu on 2021/9/8.
//

#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IAudioPlayer.h"
#include "IVideoView.h"
#include "IResample.h"
#include "XLog.h"
IPlayer::IPlayer() {

}

IPlayer *IPlayer::Get(unsigned char index) {
    static IPlayer p[256];
    return &p[index];
}

bool IPlayer::Open(const char *path) {
    mux.lock();
    if(!demux || !demux->Open(path))
    {
        mux.unlock();
        XLOGE("demux->Open %s failed", path);
        return false;
    }

    //解码可能不需要，解封装就是原始数据
    if(!vdecode || !vdecode->Open(demux->GetVPara(), isHardDecode))
    {
        XLOGE("vdecode->Open %s failed", path);
        //return false;
    }

    if(!adecode || !adecode->Open(demux->GetAPara()))
    {
        XLOGE("adecode->Open %s failed", path);
        //return false;
    }

    //重采样  解码后或者解封装后就是直接播放的数据
    //if(outParam.sample_rate == 0)
    outParam = demux->GetAPara();
    if(!resample || !resample->Open(demux->GetAPara(), outParam))
    {
        XLOGE("vdecode->Open %s failed", path);
    }
    mux.unlock();
    return true;
}

bool IPlayer::Start() {
    mux.lock();
    //先启动解码线程取视频帧数据，防止丢失
    if(audioPlayer)
        audioPlayer->StartPlay(outParam);
    if(vdecode)
        vdecode->Start();
    if(adecode)
        adecode->Start();
    if(!demux || !demux->Start())
    {
        mux.unlock();
        XLOGE("demux->Start failed");
        return false;
    }

    XThread::Start();
    mux.unlock();
    return true;
}

void IPlayer::InitView(void *win) {
    if(videoView)
    {
        videoView->Close();
        videoView->SetRender(win);
    }
}

void IPlayer::Main() {
    while (!isExit)
    {
        mux.lock();
        if(!audioPlayer || !vdecode)
        {
            mux.unlock();
            XSleep(2);
            continue;
        }
        //同步音频的pts 告诉视频
        int apts = audioPlayer->pts;
        vdecode->sysPts = apts;

        mux.unlock();
        XSleep(2);
    }
}

void IPlayer::Close() {
    mux.lock();
    //2 先关闭主体线程，再清理观察者
    //同步线程
    XThread::Stop();
    //解封装
    if(demux)
        demux->Stop();
    //解码
    if(vdecode)
        vdecode->Stop();
    if(adecode)
        adecode->Stop();
    if(audioPlayer)
        audioPlayer->Stop();

    //2 清理缓冲队列
    if(vdecode)
        vdecode->Clear();
    if(adecode)
        adecode->Clear();
    if(audioPlayer)
        audioPlayer->Clear();

    //3 清理资源
    if(audioPlayer)
        audioPlayer->Close();
    if(videoView)
        videoView->Close();
    if(vdecode)
        vdecode->Close();
    if(adecode)
        adecode->Close();
    if(demux)
        demux->Close();
    mux.unlock();
}

double IPlayer::PlayPos() {
    double pos = 0.0;
    mux.lock();

    int total = 0;
    if(demux)
        total = demux->totalMs;
    if(total>0)
    {
        if(vdecode)
        {
            pos = (double)vdecode->pts/(double)total;
        }
    }
    mux.unlock();
    return pos;
}

bool IPlayer::Seek(double pos) {
    bool re = false;
    if(!demux) return false;

    //暂停所有线程
    SetPause(true);
    mux.lock();
    //清理缓冲
    //2 清理缓冲队列
    if(vdecode)
        vdecode->Clear(); //清理缓冲队列，清理ffmpeg的缓冲
    if(adecode)
        adecode->Clear();
    if(audioPlayer)
        audioPlayer->Clear();


    re = demux->Seek(pos); //seek跳转到关键帧
    if(!vdecode)
    {
        mux.unlock();
        SetPause(false);
        return re;
    }
    //解码到实际需要显示的帧
    int seekPts = pos*demux->totalMs;
    while(!isExit)
    {
        XData pkt = demux->Read();
        if(pkt.size<=0)break;
        if(pkt.isAudio)
        {
            if(pkt.pts < seekPts)
            {
                pkt.Drop();
                continue;
            }
            //写入缓冲队列
            demux->Notify(pkt);
            continue;
        }

        //解码需要显示的帧之前的数据
        vdecode->SendPacket(pkt);
        pkt.Drop();
        XData data = vdecode->RecvFrame();
        if(data.size <=0)
        {
            continue;
        }
        if(data.pts >= seekPts)
        {
            //vdecode->Notify(data);
            break;
        }
    }
    mux.unlock();

    SetPause(false);
    return re;
}

void IPlayer::SetPause(bool isP) {
    mux.lock();
    XThread::SetPause(isP);
    if(demux)
        demux->SetPause(isP);
    if(vdecode)
        vdecode->SetPause(isP);
    if(adecode)
        adecode->SetPause(isP);
    if(audioPlayer)
        audioPlayer->SetPause(isP);
    mux.unlock();
}

void IPlayer::SetHardDecode(bool bHard) {
    mux.lock();
    isHardDecode = bHard;
    XLOGE("设置硬解码使用为 %d",isHardDecode);
    mux.unlock();
}
