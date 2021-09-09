//
// Created by zhou.lu on 2021/9/1.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::Main() {
    while (!isExit)
    {
        if(IsPause())
        {
            XSleep(2);
            continue;
        }

        XData d = Read();
        if(d.size > 0)
        {
            Notify(d);
        }
        else
            XSleep(2);
        //XLOGI("IDemux Read data size is %d", d.size);
    }
}