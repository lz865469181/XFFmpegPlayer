#include "XDecodeThread.h"
#include "XDecode.h"
XDecodeThread::XDecodeThread()
	:decode(NULL)
{
	if (!decode)
		decode = new XDecode();
}

XDecodeThread::~XDecodeThread()
{
	//等待线程退出
	isExit = true;
	wait();
} 

void XDecodeThread::Push(AVPacket* pkt)
{
	if (!pkt)
		return;

	//阻塞
	while (!isExit)
	{
		dmux.lock();
		if (packList.size() < nMaxList)
		{
			packList.push_back(pkt);
			dmux.unlock();
			break;
		}
		dmux.unlock();
		msleep(1);
	}
}

void XDecodeThread::Clear()
{
	dmux.lock();
	decode->Clear();
	while (!packList.empty())
	{
		AVPacket* pkt = packList.front();
		XFreePacket(&pkt);
		packList.pop_front();
	}

	dmux.unlock();
}

void XDecodeThread::Close()
{	
	Clear();

	//等待线程退出
	isExit = true;
	wait();
	decode->Close();

	dmux.lock();
	delete decode;
	decode = NULL;
	dmux.unlock();
}

AVPacket* XDecodeThread::Pop()
{
	dmux.lock();
	if (packList.empty())
	{
		dmux.unlock();
		return NULL;
	}

	AVPacket* pkt = packList.front();
	packList.pop_front();
	dmux.unlock();
	return pkt;
}
