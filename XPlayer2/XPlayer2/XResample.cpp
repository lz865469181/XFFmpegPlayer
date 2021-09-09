#include "XResample.h"
extern "C"
{
#include "libswresample/swresample.h"
#include "libavcodec/avcodec.h"
}
#pragma comment(lib,"swresample.lib")
XResample::XResample()
{
	swrCtx = NULL;
}

XResample::~XResample()
{

}

bool XResample::Open(AVCodecParameters* para, bool isClearPara)
{
	if (!para)
		return false;

	mux.lock();
	swrCtx = swr_alloc();
	swr_alloc_set_opts(swrCtx,
		av_get_default_channel_layout(2),
		(AVSampleFormat)m_OutFormat, //输出格式
		para->sample_rate, //输出采样率
		av_get_default_channel_layout(para->channels), //输入格式
		(AVSampleFormat)para->format,
		para->sample_rate, 0, 0);

	if (isClearPara)
		avcodec_parameters_free(&para);
	//初始化
	int ret = swr_init(swrCtx);
	mux.unlock();
	if (ret != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf) - 1);
		return false;
	}

	return true;
}

void XResample::Close()
{
	if (swrCtx)
		swr_close(swrCtx);
}

int XResample::Resample(AVFrame *indata, unsigned char *data)
{
	if (!indata)
		return 0;
	if (!data)
	{
		av_frame_free(&indata);
		return 0;
	}

	uint8_t *out_buffer[2] = { 0 };
	out_buffer[0] = data;

	//将每一帧数据转换成pcm
	int ret = swr_convert(swrCtx, out_buffer, indata->nb_samples, (const uint8_t**)indata->data, indata->nb_samples);
	//获取实际的缓存大小
	int out_buffer_size = av_samples_get_buffer_size(NULL, indata->channels, indata->nb_samples, (AVSampleFormat)m_OutFormat, 1);
	av_frame_free(&indata);

	if (ret <= 0)return ret;
	return out_buffer_size;
}
