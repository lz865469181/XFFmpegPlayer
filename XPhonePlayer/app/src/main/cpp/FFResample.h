//
// Created by zhou.lu on 2021/9/2.
//

#ifndef XPHONEPLAYER_FFRESAMPLE_H
#define XPHONEPLAYER_FFRESAMPLE_H

#include "IResample.h"
struct SwrContext;
class FFResample : public IResample {
public:
    virtual bool Open(XParameter in, XParameter out = XParameter());
    virtual void Close();
    virtual XData Resample(XData indata);
protected:
    SwrContext *actx = 0;
    std::mutex mux;
};


#endif //XPHONEPLAYER_FFRESAMPLE_H
