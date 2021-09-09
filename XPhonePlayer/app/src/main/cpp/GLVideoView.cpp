//
// Created by zhou.lu on 2021/9/1.
//

#include "GLVideoView.h"
#include "XTexture.h"
#include "XLog.h"

void GLVideoView::SetRender(void *win) {
    view = win;
}

void GLVideoView::Render(XData data) {
    if(!view)
        return;

    XLOGE("GLVideoView Render");
    if(!txture)
    {
        txture = XTexture::Create();
        txture->Init(view, (XTextureType)data.format);
    }
    txture->Draw(data.yuvdatas, data.width, data.height);
}

void GLVideoView::Close() {
    mux.lock();
    if(txture)
    {
        txture->Drop();
        txture = nullptr;
    }
    mux.unlock();
}
