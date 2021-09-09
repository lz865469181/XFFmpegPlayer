#pragma once
#include <QThread>
#include <list>
#include <mutex>
#include "XDecodeThread.h"
struct AVCodecParameters;
class XResample;
class XAudioPlayer;
class XAudioThread :
	public XDecodeThread
{
public:
	XAudioThread();
	virtual ~XAudioThread();

	//打开，不管成功与否都清理
	virtual bool Open(AVCodecParameters *para, int sampleRate, int channels);

	//停止线程.清理资源
	virtual void Close();
	virtual void Clear();

	virtual void run();

	void SetPause(bool isPause);

	long long GetPts() { return pts; }

private:
	std::mutex amux;
	bool bIsPause = false;
	XAudioPlayer *ap = NULL;
	XResample *res = NULL;
	//当前音频播放的pts
	long long pts = 0;
};

