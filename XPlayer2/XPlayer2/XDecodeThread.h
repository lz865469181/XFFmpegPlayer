#pragma once
#include <QThread>
#include <list>
#include <mutex>
class XDecode;
struct AVPacket;
class XDecodeThread :
	public QThread
{
public:
	XDecodeThread();
	virtual ~XDecodeThread();

public:
	//数据传入队列处理
	virtual void Push(AVPacket* pkt);

	//Seek位置的时候需要将当前队列的数据清空
	virtual void Clear();

	//清理资源，关闭线程
	virtual void Close();

	//去除一帧数据，并出栈，如果没有返回NULL
	virtual AVPacket* Pop();

protected:
	XDecode* decode;
	//最大队列
	int nMaxList = 100;
	bool isExit = false;
	std::list<AVPacket*> packList;
	std::mutex dmux;
};

