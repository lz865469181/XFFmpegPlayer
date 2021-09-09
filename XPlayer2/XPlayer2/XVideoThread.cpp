#include "XVideoThread.h"
#include "XDecode.h"
extern "C"
{
#include "libavcodec/avcodec.h"
}
XVideoThread::XVideoThread()
{

}

XVideoThread::~XVideoThread()
{

}

bool XVideoThread::Repaint(AVPacket* pkt, long long seekpts)
{
	vmux.lock();
	bool re = decode->SendPkt(pkt);
	if (!re)
	{
		vmux.unlock();
		return false;
	}

	AVFrame* frame = decode->RecvPkt();
	if (!frame) 
	{
		vmux.unlock();
		return false;
	}

	//����λ��
	if (decode->m_curPts >= seekpts)
	{
		if (call)
			call->Repaint(frame);
		vmux.unlock();
		return true;
	}

	XFreeFrame(&frame);
	vmux.unlock();
	return false;
}

bool XVideoThread::Open(AVCodecParameters* para, IVideoCall *call, int width, int height)
{
	if (!para)
		return false;
	Clear();

	vmux.lock();
	synpts = 0;
	//�ص���ֵ
	this->call = call;
	if (call)
		call->Init(width, height);
	vmux.unlock();

	if (!decode->Open(para))
	{
		return false;
	}
	return true;
}

void XVideoThread::run()
{
	while (!isExit)
	{
		vmux.lock();
		if (isPause)
		{
			vmux.unlock();
			msleep(5);
			continue;
		}

		if (synpts > 0 && synpts < decode->m_curPts)
		{
			vmux.unlock();
			msleep(5);
			continue;
		}

		AVPacket* pkt = Pop();
		bool re = decode->SendPkt(pkt);
		if (!re)
		{
			vmux.unlock();
			msleep(5);
			continue;
		}
		
		//��ν���
		while (!isExit)
		{
			AVFrame* frame = decode->RecvPkt();
			if(!frame)
				break;

			lastpts = frame->pts;
			//�˴���ʼ����֡������ʾ��Ƶ
			if (this->call)
				this->call->Repaint(frame);

		}
		vmux.unlock();
	}
}

void XVideoThread::SetPause(bool isPause)
{
	vmux.lock();
	this->isPause = isPause;
	vmux.unlock();	
}
