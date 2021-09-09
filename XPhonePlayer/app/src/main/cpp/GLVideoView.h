//
// Created by zhou.lu on 2021/9/1.
//

#ifndef XPHONEPLAYER_GLVIDEOVIEW_H
#define XPHONEPLAYER_GLVIDEOVIEW_H


#include "IVideoView.h"
class XTexture;
class GLVideoView : public IVideoView {
public:
    virtual void SetRender(void* win) ;
    virtual void Render(XData data) ;
    virtual void Close();
protected:
    void* view = 0;
    XTexture* txture = 0;
    std::mutex mux;
};


#endif //XPHONEPLAYER_GLVIDEOVIEW_H
