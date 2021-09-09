//
// Created by zhou.lu on 2021/9/1.
//

#ifndef XPHONEPLAYER_XEGL_H
#define XPHONEPLAYER_XEGL_H


class XEGL {
public:
    virtual bool Init(void* win) = 0;
    virtual void Close() = 0;
    virtual void Draw() = 0;
    static XEGL* Get();

protected:
    XEGL(){};
};


#endif //XPHONEPLAYER_XEGL_H
