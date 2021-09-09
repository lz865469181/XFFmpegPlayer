#pragma once
#include <mutex>
struct AVCodecParameters;
struct SwrContext;
struct AVFrame;
class XResample
{
public:
	XResample();
	~XResample();

public:
	//������������һ�£����˲�����ʽ���������ΪS16
	virtual bool Open(AVCodecParameters* para, bool isClearPara);

	virtual void Close();
	
	//�����ز������С,���ܳɹ�����ͷ�indata�ռ�
	virtual int Resample(AVFrame *indata, unsigned char *data);

	int m_OutFormat = 1;//AV_SAMPLE_FMT_S16
private:
	SwrContext *swrCtx;
	std::mutex mux;
};

