#pragma once
struct AVCodecParameters;
struct AVPacket;
struct AVCodecContext;
struct AVFrame;
extern void XFreePacket(AVPacket **pkt);
extern void XFreeFrame(AVFrame **frame);
class XDecode
{
public:
	XDecode();
	~XDecode();
public:
	//�򿪽����������ܳɹ�����ͷ�para�Ŀռ�
	virtual bool Open(AVCodecParameters* para);

	//���͵������̣߳����ܳɹ�����ͷ�Packet�Ŀռ�
	virtual bool SendPkt(AVPacket* pkt);

	//���յ����ݰ����д���
	virtual AVFrame* RecvPkt();

	//�رս��������ͷſռ�
	virtual void Close();
	virtual void Clear();

public:
	//��¼��ǰ֡�Ľ���pts
	long long m_curPts;
private:
	AVCodecContext* pCtx;
};

