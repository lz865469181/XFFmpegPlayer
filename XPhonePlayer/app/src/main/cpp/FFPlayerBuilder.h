//
// Created by zhou.lu on 2021/9/8.
//

#ifndef XPHONEPLAYER_FFPLAYERBUILDER_H
#define XPHONEPLAYER_FFPLAYERBUILDER_H
#include "IPlayerBuilder.h"

class FFPlayerBuilder: public IPlayerBuilder {
public:
    static  void InitHard(void* vm);
    static FFPlayerBuilder *Get()
    {
        static FFPlayerBuilder ff;
        return &ff;
    }
protected:
    FFPlayerBuilder();
    virtual IDemux *CreateDemux();
    virtual IDecode *CreateDecode();
    virtual IResample *CreateResample();
    virtual IAudioPlayer *CreateAudioPlay();
    virtual IVideoView *CreateVideoView();
    virtual IPlayer *CreatePlayer(unsigned char index);
};


#endif //XPHONEPLAYER_FFPLAYERBUILDER_H
