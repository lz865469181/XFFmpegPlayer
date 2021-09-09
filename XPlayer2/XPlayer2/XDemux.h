#pragma once
#include <mutex>

struct AVPacket;
struct AVFormatContext;
struct AVDictionary;
struct AVCodecParameters;
class XDemux
{
public:
	XDemux();
	virtual ~XDemux();

public:
	//初始化部分参数
	virtual bool Init();

	//打开媒体文件，或者流媒体文件
	virtual bool Open(const char* url);

	//关闭媒体文件
	virtual void Close();
	//清空当前参数
	virtual void Clear();

	//定位到视频中的某一个时间点
	virtual bool Seek(double pos);

	//获取视频返回的参数 返回的空间需要清理
	virtual AVCodecParameters* CopyVParam();
	virtual AVCodecParameters* CopyAParam();

	//判断是否是音频
	virtual bool IsAudio(AVPacket* pkt);

	//读取视频包Pkt
	virtual AVPacket* ReadVideoPkt();

	//读取所有数据包
	virtual AVPacket* Read();

	int GetWidth() { return m_nWidth; }
	int Getheight() { return m_nHeight; }

	int GetSampleRate() { return m_nSampleRate; }
	int GetChannels() { return m_nChannels; }
	int GetTotalMs() { return m_totalMs; }
private:
	AVFormatContext *pCtx = NULL;
	AVDictionary *opt = NULL;

	//当前视频的总时长
	int		m_totalMs;

	//视频流ID
	int videoStreamId;
	//视频帧的宽高，作为渲染窗口使用
	int m_nWidth;
	int m_nHeight;
	
	//音频流ID
	int audioStreamId;
	//采样率和通道数
	int m_nSampleRate;
	int m_nChannels;

};

