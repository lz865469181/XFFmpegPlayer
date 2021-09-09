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
	//�������󲢴�
	virtual bool Open(const char* url, IVideoCall* call);

	//���������߳�
	virtual void Start();

	//�ر������߳�
	virtual void Stop();
	virtual void Clear();

	//��λ��
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

