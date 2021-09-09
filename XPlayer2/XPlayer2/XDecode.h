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
	//打开解码器，不管成功与否释放para的空间
	virtual bool Open(AVCodecParameters* para);

	//发送到解码线程，不管成功与否释放Packet的空间
	virtual bool SendPkt(AVPacket* pkt);

	//接收到数据包进行处理
	virtual AVFrame* RecvPkt();

	//关闭解码器，释放空间
	virtual void Close();
	virtual void Clear();

public:
	//记录当前帧的解码pts
	long long m_curPts;
private:
	AVCodecContext* pCtx;
};

