#include "XDemuxThread.h"
#include "XDemux.h"
#include "XVideoThread.h"
#include "XAudioThread.h"
extern "C"
{
#include "libavcodec/avcodec.h"
}

XDemuxThread::XDemuxThread()
{

}

XDemuxThread::~XDemuxThread()
{
	isExit = true;
	wait();
}

bool XDemuxThread::Open(const char* url, IVideoCall* call)
{
	if (url == NULL || url[0] == '\0')
		return false;

	mux.lock();
	if (!demux)
		demux = new XDemux();
	if (!vt)
		vt = new XVideoThread();
	if (!at)
		at = new XAudioThread();

	//�򿪽��װ
	bool ret = demux->Open(url);
	if (!ret)
	{
		mux.unlock();
		return false;
	}

	//����Ƶ�����̺߳ʹ����߳�
	if (!vt->Open(demux->CopyVParam(), call, demux->GetWidth(), demux->Getheight()))
	{
		mux.unlock();
		ret = false;
	}

	//����Ƶ�����߳�
	if (!at->Open(demux->CopyAParam(), demux->GetSampleRate(), demux->GetChannels()))
	{
		mux.unlock();
		ret = false;
	}

	totalMs = demux->GetTotalMs();
	mux.unlock();

	return ret;
}

void XDemuxThread::Start()
{
	mux.lock();
	if (!demux)
		demux = new XDemux();
	if (!vt)
		vt = new XVideoThread();
	if (!at)
		at = new XAudioThread();

	//������ǰ����
	QThread::start();
	if (vt)
		vt->start();
	if (at)
		at->start();
	mux.unlock();
}

void XDemuxThread::Stop()
{
	isExit = true;
	wait();
	
	if (vt)
		vt->Close();
	if (at)
		at->Close();

	mux.lock();
	delete vt;
	vt = NULL;
	delete at;
	at = NULL;
	mux.unlock();
}

void XDemuxThread::Clear()
{
	mux.lock();
	if (demux) demux->Clear();
	if (vt) vt->Clear();
	if (at) at->Clear();
	mux.unlock();
}

void XDemuxThread::Seek(double pos)
{
	//������
	Clear();

	mux.lock();
	bool status = this->isPause;
	mux.unlock();
	//��ͣ ��λ��Ƶʱ����ͣ�ڲ��ţ���Ȼҳ�����˸
	SetPause(true);

	mux.lock();
	if (demux)
		demux->Seek(pos);

	//ʵ����Ҫ��ʾ��λ��
	long long seekPts = demux->GetTotalMs() * pos;

	while (!isExit)
	{
		AVPacket* pkt = demux->ReadVideoPkt();
		if(!pkt)
			break;
		//������뵽seekPts
		if (vt->Repaint(pkt, seekPts))
		{
			this->pts = seekPts;
			break;
		}
	}
}

void XDemuxThread::run()
{
	while (!isExit)
	{
		mux.lock();
		if (isPause)
		{
			mux.unlock();
			msleep(5);
			continue;
		}
		if (!demux)
		{
			mux.unlock();
			msleep(5);
			continue;
		}

		//����Ƶͬ������ ����Ƶ��Ϊ����
		/*if (vt && at)
		{
			pts = at->GetPts();
			vt->synpts = at->GetPts();
		}*/
		
		if (at && vt)
		{
			pts - 
		}

		AVPacket* pkt = demux->Read();

		//�ж�����Ƶ
		if (demux->IsAudio(pkt))
		{
			if(at)
				at->Push(pkt);
		}
		else {
			if (vt)
				vt->Push(pkt);
		}

		mux.unlock();
		msleep(1);
	}
}

void XDemuxThread::SetPause(bool isPause)
{
	this->isPause = isPause;
	if (vt)
		vt->SetPause(isPause);
	if (at) at->SetPause(isPause);
}
