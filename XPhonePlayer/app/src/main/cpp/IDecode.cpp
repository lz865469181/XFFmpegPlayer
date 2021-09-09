//
// Created by zhou.lu on 2021/9/1.
//

#include "IDecode.h"
#include "XLog.h"

void IDecode::Main() {
    while (!isExit)
    {
        if(IsPause())
        {
            XSleep(2);
            continue;
        }

        packsMutex.lock();
        //判断音视频同步
        if(!isAudio && sysPts > 0)
        {
            if(sysPts < pts)
            {
                packsMutex.unlock();
                XSleep(1);
                continue;
            }

        }

        if(packs.empty())
        {
            packsMutex.unlock();
            XSleep(1);
            continue;
        }
        //取出pakcet
        XData pack = packs.front();
        packs.pop_front();
        //消费者
        //发送数据到解码线程 一个数据包，可解码多个结果
        if(this->SendPacket(pack))
        {
            while (!isExit)
            {
                //获取解码数据
                XData frame = RecvFrame();
                if(!frame.data)
                    break;

                pts = frame.pts;
                //发送数据给观察者
                this->Notify(frame);
            }
        }
        //必须操作，不然容易内存泄漏
        pack.Drop();
        packsMutex.unlock();
    }
}

void IDecode::Update(XData pkt) {
    if(pkt.isAudio != isAudio)
        return;

    while (!isExit)
    {
        packsMutex.lock();

        //阻塞
        if(packs.size() < maxList)
        {//生产者
            packs.push_back(pkt);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        XSleep(1);
    }
}

void IDecode::Clear() {
    packsMutex.lock();
    while (!packs.empty())
    {
        packs.front().Drop();
        packs.pop_front();
    }
    pts = 0;
    sysPts = 0;
    packsMutex.unlock();
}
