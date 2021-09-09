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
	//��ʼ�����ֲ���
	virtual bool Init();

	//��ý���ļ���������ý���ļ�
	virtual bool Open(const char* url);

	//�ر�ý���ļ�
	virtual void Close();
	//��յ�ǰ����
	virtual void Clear();

	//��λ����Ƶ�е�ĳһ��ʱ���
	virtual bool Seek(double pos);

	//��ȡ��Ƶ���صĲ��� ���صĿռ���Ҫ����
	virtual AVCodecParameters* CopyVParam();
	virtual AVCodecParameters* CopyAParam();

	//�ж��Ƿ�����Ƶ
	virtual bool IsAudio(AVPacket* pkt);

	//��ȡ��Ƶ��Pkt
	virtual AVPacket* ReadVideoPkt();

	//��ȡ�������ݰ�
	virtual AVPacket* Read();

	int GetWidth() { return m_nWidth; }
	int Getheight() { return m_nHeight; }

	int GetSampleRate() { return m_nSampleRate; }
	int GetChannels() { return m_nChannels; }
	int GetTotalMs() { return m_totalMs; }
private:
	AVFormatContext *pCtx = NULL;
	AVDictionary *opt = NULL;

	//��ǰ��Ƶ����ʱ��
	int		m_totalMs;

	//��Ƶ��ID
	int videoStreamId;
	//��Ƶ֡�Ŀ�ߣ���Ϊ��Ⱦ����ʹ��
	int m_nWidth;
	int m_nHeight;
	
	//��Ƶ��ID
	int audioStreamId;
	//�����ʺ�ͨ����
	int m_nSampleRate;
	int m_nChannels;

};

