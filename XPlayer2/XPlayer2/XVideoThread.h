#pragma once
#include <QThread>
#include <list>
#include <mutex>
#include "XDecodeThread.h"
#include "IVideoCall.h"
#include <mutex>
struct AVPacket;
struct AVCodecParameters;
class XVideoThread : public XDecodeThread
{
public:
	XVideoThread();
	virtual ~XVideoThread();

public:
	//解码pts, 如果接收到的解码数据pts >= seek pts return true ,并且显示华民
	virtual bool Repaint(AVPacket* pkt, long long seekpts);

	//打开，不管成功与否
	virtual bool Open(AVCodecParameters* para, IVideoCall *call, int width, int height);

	//线程重写方法
	virtual void run();

	//同步时间，由外部传入
	long long synpts = 0;

	void SetPause(bool isPause);
	bool isPause = false;

	int64_t GetLastPts()
	{
		return lastpts;
	}
private:
	IVideoCall *call = NULL;
	std::mutex vmux;
	int64_t lastpts = 0.0;
};

