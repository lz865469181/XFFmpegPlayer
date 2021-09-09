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
	//���ݴ�����д���
	virtual void Push(AVPacket* pkt);

	//Seekλ�õ�ʱ����Ҫ����ǰ���е��������
	virtual void Clear();

	//������Դ���ر��߳�
	virtual void Close();

	//ȥ��һ֡���ݣ�����ջ�����û�з���NULL
	virtual AVPacket* Pop();

protected:
	XDecode* decode;
	//������
	int nMaxList = 100;
	bool isExit = false;
	std::list<AVPacket*> packList;
	std::mutex dmux;
};

