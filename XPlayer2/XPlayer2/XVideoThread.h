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
	//����pts, ������յ��Ľ�������pts >= seek pts return true ,������ʾ����
	virtual bool Repaint(AVPacket* pkt, long long seekpts);

	//�򿪣����ܳɹ����
	virtual bool Open(AVCodecParameters* para, IVideoCall *call, int width, int height);

	//�߳���д����
	virtual void run();

	//ͬ��ʱ�䣬���ⲿ����
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

