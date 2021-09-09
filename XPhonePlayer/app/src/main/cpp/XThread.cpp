//
// Created by zhou.lu on 2021/9/1.
//

#include "XThread.h"
#include "XLog.h"
#include <thread>
using namespace std;
//启动线程
bool XThread::Start() {
    isExit = false;
    isPause = false;
    thread th(&XThread::ThreadMain, this);
    th.detach();
    return true;
}

void XThread::ThreadMain() {
    XLOGI("线程函数进入");
    isRunning = true;
    Main();
    isRunning = false;
    XLOGI("线程函数退出");
}

void XThread::Stop() {
    isExit = true;
    for(int i = 0; i< 200; i++) {
        if(!isRunning) {
            XLOGI("停止线程成功");
            break;
        }

        XSleep(1);
    }
    XLOGI("停止线程超时");
}

void XThread::SetPause(bool isP) {
    isPause = isP;
    //等待100毫秒
    for(int i = 0; i < 10; i++)
    {
        if(isPausing == isP)
        {
            break;
        }
        XSleep(10);
    }
}

void XSleep(int mis)
{
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}