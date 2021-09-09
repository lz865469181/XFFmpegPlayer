//
// Created by zhou.lu on 2021/9/1.
//

#ifndef XPHONEPLAYER_XTHREAD_H
#define XPHONEPLAYER_XTHREAD_H
void XSleep(int mis);
//使用C++11线程库
class XThread{
public:
    //启动线程
    virtual bool Start();
    //通过控制isExit 安全停止线程
    virtual void Stop();

    virtual void SetPause(bool isP);

    virtual bool IsPause()
    {
        isPausing = isPause;
        return isPausing;
    }

    //入口主函数
    virtual void Main(){}

protected:
    bool isExit = false;
    bool isRunning = false;
    bool isPause = false;
    bool isPausing = false;
private:
    void ThreadMain();
};


#endif //XPHONEPLAYER_XTHREAD_H
