//
// Created by zhou.lu on 2021/9/1.
//

#ifndef XPHONEPLAYER_XLOG_H
#define XPHONEPLAYER_XLOG_H



class XLog {

};
#ifdef ANDROID

#include <android/log.h>
#define XLOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"XPhonePlay", __VA_ARGS__)
#define XLOGI(...)  __android_log_print(ANDROID_LOG_INFO,"XPhonePlay", __VA_ARGS__)
#define XLOGE(...)  __android_log_print(ANDROID_LOG_ERROR,"XPhonePlay", __VA_ARGS__)
#else
#define XLOGD(...)  print("XPhonePlay",__VA_ARGS__)
#define XLOGI(...)  print("XPhonePlay",__VA_ARGS__)
#define XLOGE(...)  print("XPhonePlay",__VA_ARGS__)
#endif

#endif //XPHONEPLAYER_XLOG_H
