//
// Created by zhou.lu on 2021/9/1.
//

#ifndef XPHONEPLAYER_XPARAMETER_H
#define XPHONEPLAYER_XPARAMETER_H

struct AVCodecParameters;
class XParameter {
public:
    AVCodecParameters *para = 0;
    int channels = 2;
    int sample_rate = 44100;
};


#endif //XPHONEPLAYER_XPARAMETER_H
