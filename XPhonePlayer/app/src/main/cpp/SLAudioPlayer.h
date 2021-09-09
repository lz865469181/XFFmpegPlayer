//
// Created by zhou.lu on 2021/9/2.
//

#ifndef XPHONEPLAYER_SLAUDIOPLAYER_H
#define XPHONEPLAYER_SLAUDIOPLAYER_H


#include "IAudioPlayer.h"

class SLAudioPlayer : public IAudioPlayer {
public:
    virtual bool StartPlay(XParameter out);
    virtual void Close();
    void PlayCall(void *bufq);

    SLAudioPlayer();
    virtual ~SLAudioPlayer();

protected:
    unsigned char *buf = 0;
    std::mutex mux;
};


#endif //XPHONEPLAYER_SLAUDIOPLAYER_H
