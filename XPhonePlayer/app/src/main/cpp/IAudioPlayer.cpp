//
// Created by zhou.lu on 2021/9/2.
//

#include "IAudioPlayer.h"

void IAudioPlayer::Upadte(XData data) {
    //压入缓冲队列
    if(data.size < 0 || !data.data)
        return;
    while (!isExit)
    {
        framesMutex.lock();
        if(frames.size() > maxFrame)
        {
            framesMutex.unlock();
            XSleep(1);
            return;
        }
        frames.push_back(data);
        framesMutex.unlock();
        break;
    }
}

XData IAudioPlayer::GetData() {
    XData d;

    isRunning = true;
    while (!isExit)
    {
        if(IsPause())
        {
            XSleep(2);
            continue;
        }

        framesMutex.lock();
        if(!frames.empty())
        {
            //有数据
            d = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            pts = d.pts;
            return d;
        }
        framesMutex.unlock();
        XSleep(1);
    }
    isRunning = false;
    //pts = d.pts;

    //未找到异常
    return d;
}

void IAudioPlayer::Clear() {
    framesMutex.lock();
    while (!frames.empty())
    {
        frames.front().Drop();
        frames.pop_front();
    }
    framesMutex.unlock();
}
