//
// Created by zhou.lu on 2021/9/1.
//

#include "XTexture.h"
#include "XLog.h"
#include "XEGL.h"
#include "XShader.h"

class CXTexture: public XTexture
{
public:
    XShader shader;
    XTextureType type;
    std::mutex mux;
    virtual void Drop() {
        mux.lock();
        XEGL::Get()->Close();
        shader.Close();
        mux.unlock();
        delete this;
    }

    virtual bool Init(void* win, XTextureType type=XTEXTURE_YUV420P)
    {
        mux.lock();
        XEGL::Get()->Close();
        shader.Close();
        this->type = type;
        if(!win) {
            mux.unlock();
           XLOGE("XTexture Init failed win is NULL");
           return false;
        }
        if(!XEGL::Get()->Init(win)) {
            mux.unlock();
            return false;
        }
        shader.Init((XShaderType)type);
        mux.unlock();
        return false;
    }

    virtual bool Draw(unsigned char* data[], int width, int height)
    {
        mux.lock();
        shader.GetTexture(0, width, height, data[0]); //Y
        if(type == XTEXTURE_YUV420P)
        {
            shader.GetTexture(1,width/2,height/2,data[1]);  // U
            shader.GetTexture(2,width/2,height/2,data[2]);  // V
        }
        else
        {
            shader.GetTexture(1,width/2,height/2,data[1], true);  // UV
        }
        shader.Draw();

        XEGL::Get()->Draw();
        mux.unlock();
    }
};

XTexture *XTexture::Create() {
    return new CXTexture();
}