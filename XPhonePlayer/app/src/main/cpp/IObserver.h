//
// Created by zhou.lu on 2021/9/1.
//

#ifndef XPHONEPLAYER_IOBSERVER_H
#define XPHONEPLAYER_IOBSERVER_H
#include <vector>
#include <mutex>
#include "XData.h"
#include "XThread.h"

//包含观察者和主体
class IObserver : public XThread {
public:
    //观察者接受数据函数
    virtual void Update(XData data) {};

    //主体函数 添加观察者 线程安全
    void AddObs(IObserver* obs);

    //通知所有观察者 线程安全
    void Notify(XData data);

protected:
    std::vector<IObserver *> obss;
    std::mutex mux;
};


#endif //XPHONEPLAYER_IOBSERVER_H
