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

	//�򿪣����ܳɹ��������
	virtual bool Open(AVCodecParameters *para, int sampleRate, int channels);

	//ֹͣ�߳�.������Դ
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
	//��ǰ��Ƶ���ŵ�pts
	long long pts = 0;
};

