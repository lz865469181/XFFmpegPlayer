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
	//输入和输出参数一致，除了采样格式的区别，输出为S16
	virtual bool Open(AVCodecParameters* para, bool isClearPara);

	virtual void Close();
	
	//返回重采样后大小,不管成功与否都释放indata空间
	virtual int Resample(AVFrame *indata, unsigned char *data);

	int m_OutFormat = 1;//AV_SAMPLE_FMT_S16
private:
	SwrContext *swrCtx;
	std::mutex mux;
};

