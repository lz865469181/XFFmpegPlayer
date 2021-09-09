//
// Created by zhou.lu on 2021/9/2.
//

#ifndef XPHONEPLAYER_IAUDIOPLAYER_H
#define XPHONEPLAYER_IAUDIOPLAYER_H


#include "IObserver.h"
#include "XParameter.h"
#include <list>

class IAudioPlayer : public IObserver {
public:
    //缓冲满后阻塞
    virtual void Upadte(XData data);

    //获取缓冲数据,如没有则阻塞
    virtual XData GetData();

    virtual bool StartPlay(XParameter out) = 0;
    virtual void Close() = 0;
    virtual void Clear();
    //最大缓冲
    int maxFrame = 100;
    int pts = 0;
protected:
    std::list<XData> frames;
    std::mutex framesMutex;
};


#endif //XPHONEPLAYER_IAUDIOPLAYER_H
