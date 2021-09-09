//
// Created by zhou.lu on 2021/9/8.
//

#include "FFPlayerBuilder.h"
#include "FFDemux.h"
#include "FFDecode.h"
#include "FFResample.h"
#include "SLAudioPlayer.h"
#include "GLVideoView.h"

IDemux *FFPlayerBuilder::CreateDemux() {
    IDemux* ff = new FFDemux();
    return ff;
}

IDecode *FFPlayerBuilder::CreateDecode() {
    IDecode* ff = new FFDecode();
    return ff;
}

IResample *FFPlayerBuilder::CreateResample() {
    IResample* ff = new FFResample();
    return ff;
}

IAudioPlayer *FFPlayerBuilder::CreateAudioPlay() {
    IAudioPlayer* ff = new SLAudioPlayer();
    return ff;
}

IVideoView *FFPlayerBuilder::CreateVideoView() {
    IVideoView* ff = new GLVideoView();
    return ff;
}

IPlayer *FFPlayerBuilder::CreatePlayer(unsigned char index) {
    return IPlayer::Get(index);
}

FFPlayerBuilder::FFPlayerBuilder() {

}

void FFPlayerBuilder::InitHard(void *vm) {
    FFDecode::InitHard(vm);
}
