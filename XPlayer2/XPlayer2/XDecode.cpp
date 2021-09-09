#include "XDecode.h"
extern "C"
{
#include "libavcodec/avcodec.h"
}

extern void XFreePacket(AVPacket **pkt)
{
	if (!pkt || !(*pkt))return;
	av_packet_free(pkt);
}

extern void XFreeFrame(AVFrame **frame)
{
	if (!frame || !(*frame))return;
	av_frame_free(frame);
}

XDecode::XDecode()
{
	pCtx = NULL;
}

XDecode::~XDecode()
{

}

bool XDecode::Open(AVCodecParameters* para)
{
	if (!para)
		return false;
	Close();

	//�ҵ���Ӧ�Ľ�����
	AVCodec* vCodec = avcodec_find_decoder(para->codec_id);
	if (!vCodec)
	{
		//�ͷŶ�Ӧ�Ľ���������
		avcodec_parameters_free(&para);
		return false;
	}
	
	//��������������Ŀռ�
	pCtx = avcodec_alloc_context3(vCodec);
	//������������Ƹ������Ŀռ�
	avcodec_parameters_to_context(pCtx, para);
	//�ͷŲ����ռ�
	avcodec_parameters_free(&para);

	//�����Ŀռ����ý����߳���
	pCtx->thread_count = 8;

	//�򿪽��������� vCodec Ҫô��NULL Ҫô��֮ǰ�Ĵ������һ��
	int ret = avcodec_open2(pCtx, vCodec, NULL);
	if (ret != 0)
	{
		avcodec_free_context(&pCtx);
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf) - 1);
		return false;
	}

	return true;
}

bool XDecode::SendPkt(AVPacket* pkt)
{
	if (!pkt || (pkt->size == 0) || (!pkt->data))
		return false;
	if (!pCtx)
		return false;
	int ret = avcodec_send_packet(pCtx, pkt);
	if (ret != 0)
	{
		av_packet_free(&pkt);
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf) - 1);
		return false;
	}

	return true;
}

AVFrame* XDecode::RecvPkt()
{
	if (!pCtx)
		return false;
	//��ʼ������֡
	AVFrame* frame = av_frame_alloc();
	int ret = avcodec_receive_frame(pCtx, frame);
	if (ret != 0)
	{
		av_frame_free(&frame);
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf) - 1);
		return false;
	}
	
	m_curPts = frame->pts;
	return frame;
}

void XDecode::Close()
{
	if (!pCtx)
		return;
	avcodec_close(pCtx);
	avcodec_free_context(&pCtx);
	return;
}

void XDecode::Clear()
{
	//������뻺��
	if (pCtx)
		avcodec_flush_buffers(pCtx);
}
