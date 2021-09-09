#include "XAudioThread.h"
#include "XAudioPlayer.h"
#include "XResample.h"
#include "XDecode.h"
XAudioThread::XAudioThread()
{
	if (!res)
		res = new XResample();
	if (!ap)
		ap = XAudioPlayer::Get();
}

XAudioThread::~XAudioThread()
{

}

bool XAudioThread::Open(AVCodecParameters *para, int sampleRate, int channels)
{
	if (!para)
		return false;

	amux.lock();
	pts = 0;
	bool isOpen = true;
	if (!res->Open(para, false))
	{
		isOpen = false;
	}
	ap->SetSampleRate(sampleRate);
	ap->SetChannel(channels);

	if (!ap->Open())
	{
		isOpen = false;
	}
	if (!decode->Open(para))
	{
		isOpen = false;
	}
	amux.unlock();

	return isOpen;
}

void XAudioThread::Close()
{
	Clear();
	//�ر��߳�
	isExit = true;
	wait();

	amux.lock();
	delete decode;
	decode = NULL;
	amux.unlock();
}

void XAudioThread::Clear()
{
	XDecodeThread::Clear();
	amux.lock();
	if (ap)
		ap->Clear();
	amux.unlock();
}

void XAudioThread::run()
{
	unsigned char* pcm = new unsigned char[1024 * 1024*10]; //10M��С
	while (!isExit)
	{
		amux.lock();
		if (bIsPause)
		{
			amux.unlock();
			msleep(5);
			continue;
		}

		//��ȡ��ǰ�����ݰ�
		AVPacket* pkt = Pop();
		bool ret = decode->SendPkt(pkt);
		if (!ret)
		{
			//�Ҳ�����Ƶ���ݰ����ȴ�һ�Ỻ��
			amux.unlock();
			msleep(1);
			continue;
		}

		//һ��Send ѭ������Recv
		while (!isExit)
		{
			AVFrame* frame = decode->RecvPkt();
			if(!frame)
				break;

			//��ȥ������δ���ŵ�ʱ��
			pts = decode->m_curPts - ((QTAudioPlayer*)ap)->GetNoPlayMs();
			//�ز�����ò�������
			int size = res->Resample(frame, pcm);

			while (!isExit)
			{
				if(size <= 0)
					break;
				//����δ�����꣬�ռ䲻��
				if (((QTAudioPlayer*)ap)->GetFree() < size || bIsPause)
				{
					msleep(5);
					continue;
				}
				ap->WriteBuf(pcm, size);
				break;
			}
		}
		amux.unlock();
	}

	delete pcm;
}

void XAudioThread::SetPause(bool isPause)
{
	bIsPause = isPause;
	if (ap)
		ap->SetPause(bIsPause);
}
