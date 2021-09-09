//
// Created by zhou.lu on 2021/9/2.
//

#include "IResample.h"

void IResample::Update(XData data) {
    XData d = this->Resample(data);
    if(d.size > 0)
    {
        this->Notify(d);
    }
}
