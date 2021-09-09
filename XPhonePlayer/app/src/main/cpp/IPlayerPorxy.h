//
// Created by zhou.lu on 2021/9/8.
//

#ifndef XPHONEPLAYER_IPLAYERPORXY_H
#define XPHONEPLAYER_IPLAYERPORXY_H

#include "IPlayer.h"
#include <mutex>
class IPlayerPorxy : public IPlayer {
public:
    static IPlayerPorxy *Get()
    {
        static IPlayerPorxy px;
        return &px;
    }
    void Init(void *vm = 0);

    virtual bool Open(const char* path);
    virtual void Close();
    virtual bool Start();
    virtual void InitView(void* win);

    //获取当前的播放进度 0.0 ~ 1.0
    virtual double PlayPos();
    virtual bool Seek(double pos);
    virtual void SetPause(bool isP);
    virtual bool IsPause();

    virtual void SetHardDecode(bool bHard);

protected:
    IPlayerPorxy(){}
    IPlayer *player = 0;
    std::mutex mux;
};


#endif //XPHONEPLAYER_IPLAYERPORXY_H
