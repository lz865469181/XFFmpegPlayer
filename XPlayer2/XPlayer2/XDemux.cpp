#include "XDemux.h"
#include <iostream>
using namespace std;
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
}
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")

static double r2d(AVRational r)
{
	return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

XDemux::XDemux()
{
	videoStreamId = 0;
	audioStreamId = 0;
	m_nWidth = 0;
	m_nHeight = 0;

	Init();
}

XDemux::~XDemux()
{

}

bool XDemux::Init()
{
	static bool isFirst = true;
	if (!isFirst)
		return false;
	//初始化FFMPEG  调用了这个才能正常适用编码器和解码器
	av_register_all();
	//初始化网络参数
	avformat_network_init();

	//设置部分参数
	//设置rtsp流以tcp协议打开
	av_dict_set(&opt, "rtsp_transport", "tcp", 0);
	//网络延迟时间设置
	av_dict_set(&opt, "max_delay", "500", 0);
	isFirst = false;
}

bool XDemux::Open(const char* url)
{
	//打开之前先关闭系统
	Close();

	int ret = avformat_open_input(&pCtx, url,
		0,	//表示自动寻找解码器
		&opt);

	if (ret != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf)-1);
		return false;
	}

	//获取流信息
	ret = avformat_find_stream_info(pCtx, NULL);
	m_totalMs = pCtx->duration / (AV_TIME_BASE / 1000);

	//获得视频流Index
 	videoStreamId = av_find_best_stream(pCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	//打印视频流信息
	av_dump_format(pCtx, videoStreamId, url, NULL);

	//获得视频流指针
	AVStream *vs = pCtx->streams[videoStreamId];
	//流信息中的解码器参数
	m_nWidth = vs->codecpar->width;
	m_nHeight = vs->codecpar->height;

	cout << "==================================================================" << endl;
	cout << "视频信息" << videoStreamId << endl;
	cout << "code_id = " << vs->codecpar->codec_id << endl;
	cout << "format = " << vs->codecpar->format << endl;
	cout << "width = " << m_nWidth << endl;
	cout << "height = " << m_nHeight << endl;
	cout << "video fps = " << r2d(vs->avg_frame_rate) << endl;
	cout << "video 格式 = " << pCtx->iformat->long_name << endl;

	//获取音频流信息
	audioStreamId = av_find_best_stream(pCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	AVStream *as = pCtx->streams[audioStreamId];
	m_nSampleRate = as->codecpar->sample_rate;
	m_nChannels = as->codecpar->channels;
	cout << "==================================================================" << endl;
	cout << "音频信息" << audioStreamId << endl;
	cout << "code_id=" << as->codecpar->codec_id << endl;
	cout << "format=" << as->codecpar->format << endl;
	cout << "sampleRate = " << m_nSampleRate << endl;
	cout << "channels = " << m_nChannels << endl;
	cout << "frame_size = " << as->codecpar->frame_size << endl;

	return true;
}

void XDemux::Close()
{
	if (!pCtx)
		return;
	avformat_close_input(&pCtx);
	m_nWidth = 0;
	m_nHeight = 0;
	m_nSampleRate = 0;
	m_totalMs = 0;
	m_nChannels = 0;
}

//清空读取缓存
void XDemux::Clear()
{
	if (!pCtx)
		return;
	avformat_flush(pCtx);
}

bool XDemux::Seek(double pos)
{
	if (!pCtx)
		return false;

	Clear();
	long long seekPos = 0;
	seekPos = pCtx->streams[videoStreamId]->duration*pos;
	int ret = av_seek_frame(pCtx, videoStreamId, seekPos, 0);
	if (ret != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf) - 1);
		return false;
	}

	return true;
}

AVCodecParameters* XDemux::CopyVParam()
{
	if (!pCtx)
	{
		return false;
	}

	//参数复制进行空间初始化申请
	AVCodecParameters *params = avcodec_parameters_alloc();
	avcodec_parameters_copy(params, pCtx->streams[videoStreamId]->codecpar);

	return params;
}

AVCodecParameters* XDemux::CopyAParam()
{
	if (!pCtx)
	{
		return false;
	}

	//参数复制进行空间初始化申请
	AVCodecParameters *params = avcodec_parameters_alloc();
	avcodec_parameters_copy(params, pCtx->streams[audioStreamId]->codecpar);

	return params;
}

bool XDemux::IsAudio(AVPacket* pkt)
{
	if (!pkt)
		return false;

	//
	if (pkt->stream_index == audioStreamId)
		return true;
	return false;
}

AVPacket* XDemux::ReadVideoPkt()
{
	if (!pCtx)
		return NULL;

	AVPacket* pkt = NULL;
	
	long millsec = 0;
	while (true)
	{
		pkt = Read();
		if (!pkt)
			break;
		if (pkt->stream_index != videoStreamId)
			break;
		if(millsec++ > 100)
			break;
		av_packet_free(&pkt);
	}

	return pkt;
}

//读取每一帧视频包
AVPacket* XDemux::Read()
{
	if (!pCtx)
		return NULL;

	//初始化空间
	AVPacket* pkt = av_packet_alloc();
	int ret = av_read_frame(pCtx, pkt);
	if (ret != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf) - 1);
		av_packet_free(&pkt);
		return NULL;
	}

	//将Pkt的dts转换毫秒
	pkt->dts = pkt->dts * (1000 * r2d(pCtx->streams[videoStreamId]->time_base));
	pkt->pts = pkt->pts * (1000 * r2d(pCtx->streams[videoStreamId]->time_base));

	return pkt;
}
