#pragma once
#include <QThread>
#include "IVideoCall.h"
#include <mutex>
class XDemux;
class XVideoThread;
class XAudioThread;
class XDemuxThread :
	public QThread
{
public:
	XDemuxThread();

	virtual ~XDemuxThread();
public:
	//创建对象并打开
	virtual bool Open(const char* url, IVideoCall* call);

	//启动所有线程
	virtual void Start();

	//关闭所有线程
	virtual void Stop();
	virtual void Clear();

	//定位到
	virtual void Seek(double pts);

	virtual void run();

	void SetPause(bool isPause);

public:
	bool isExit = false;
	long long pts = 0;
	long long totalMs = 0;
	bool isPause = false;

private:
	std::mutex mux;
	XDemux* demux = NULL;
	XVideoThread *vt = NULL;
	XAudioThread *at = NULL;
};

