//
// Created by zhou.lu on 2021/9/1.
//

#ifndef XPHONEPLAYER_IVIDEOVIEW_H
#define XPHONEPLAYER_IVIDEOVIEW_H


#include "XData.h"
#include "IObserver.h"

class IVideoView : public IObserver {
public:
    virtual void SetRender(void* win) = 0;
    virtual void Render(XData data) = 0;
    virtual void Update(XData data);
    virtual void Close() = 0;
};


#endif //XPHONEPLAYER_IVIDEOVIEW_H
