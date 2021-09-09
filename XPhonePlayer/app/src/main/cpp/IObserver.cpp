//
// Created by zhou.lu on 2021/9/1.
//

#include "IObserver.h"

void IObserver::AddObs(IObserver *obs) {
    //添加观察者
    if(!obs)
        return;

    mux.lock();
    obss.push_back(obs);
    mux.unlock();
}

void IObserver::Notify(XData data) {
    mux.lock();
    //使用迭代器，解决删除崩溃问题
//    std::vector<IObserver*>::iterator iter = obss.begin();
//    while (iter != obss.end())
//    {
//        (*iter)->Update(data);
//        iter++;
//    }
    for(int i =0; i < obss.size(); i++)
    {
        obss[i]->Update(data);
    }
    mux.unlock();
}
