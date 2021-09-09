//
// Created by zhou.lu on 2021/9/2.
//

#ifndef XPHONEPLAYER_IRESAMPLE_H
#define XPHONEPLAYER_IRESAMPLE_H
#include "IObserver.h"
#include "XParameter.h"
class IResample : public IObserver {
public:
    virtual bool Open(XParameter in, XParameter out = XParameter()) = 0;
    virtual XData Resample(XData indata) = 0;
    virtual void Close() = 0;
    virtual void Update(XData data);

    int outChannels = 2;
    int outFormat = 1;
};


#endif //XPHONEPLAYER_IRESAMPLE_H
