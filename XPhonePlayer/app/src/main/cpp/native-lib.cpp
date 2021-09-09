#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include "XLog.h"
#include "IPlayerPorxy.h"

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm,void *res) {
    IPlayerPorxy::Get()->Init(vm);
    IPlayerPorxy::Get()->SetHardDecode(false);
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xphoneplayer_XPlay_InitView(JNIEnv *env, jobject thiz, jobject surface) {
    // TODO: implement InitView()
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    IPlayerPorxy::Get()->InitView(win);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_xphoneplayer_OpenUrl_Open(JNIEnv *env, jobject thiz, jstring url) {
    // TODO: implement Open()
    const char* path = env->GetStringUTFChars(url, 0);

    IPlayerPorxy::Get()->Open(path);
    IPlayerPorxy::Get()->Start();

    env->ReleaseStringUTFChars(url, path);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_xphoneplayer_MainActivity_PlayPos(JNIEnv *env, jobject thiz) {
    // TODO: implement PlayPos()
    return IPlayerPorxy::Get()->PlayPos();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_xphoneplayer_MainActivity_Seek(JNIEnv *env, jobject thiz, jdouble pos) {
    // TODO: implement Seek()
    IPlayerPorxy::Get()->Seek(pos);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_xphoneplayer_XPlay_PlayOrPause(JNIEnv *env, jobject thiz) {
    IPlayerPorxy::Get()->SetPause(IPlayerPorxy::Get()->IsPause());
}