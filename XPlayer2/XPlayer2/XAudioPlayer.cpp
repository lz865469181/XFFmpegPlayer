#include "XAudioPlayer.h"
#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>
XAudioPlayer* XAudioPlayer::Get()
{
	static QTAudioPlayer audioplayer;
	return &audioplayer;
}

XAudioPlayer::XAudioPlayer()
{

}

XAudioPlayer::~XAudioPlayer()
{

}

QTAudioPlayer::QTAudioPlayer()
{
	m_nSampleRate = 44100;
	m_nChannels = 2;
	m_nSampleSize = 16;

	io = NULL;
	m_bIsPause = false;
}

QTAudioPlayer::~QTAudioPlayer()
{

}

bool QTAudioPlayer::Open()
{
	Close();
	//设置音频播放格式
	QAudioFormat fmt;
	fmt.setSampleRate(m_nSampleRate);
	fmt.setSampleSize(m_nSampleSize);
	fmt.setChannelCount(m_nChannels);
	fmt.setCodec("audio/pcm");
	fmt.setByteOrder(QAudioFormat::LittleEndian);
	fmt.setSampleType(QAudioFormat::UnSignedInt);

	output = new QAudioOutput(fmt);
	io = output->start();

	if (io)
		return true;
	return false;
}

void QTAudioPlayer::Close()
{
	if (io) {
		io->close();
		io = NULL;
	}
	if (output) {
		output->stop();
		output = NULL;
	}
}

void QTAudioPlayer::Clear()
{
	if (io)
		io->reset();
}

bool QTAudioPlayer::WriteBuf(const unsigned char *buf, int dataSize)
{
	if (!buf || dataSize <= 0)
		return false;

	if (!output || !io)
		return false;

	int nSize = io->write((char*)buf, dataSize);
	if (nSize != dataSize)
		return false;
	return true;
}

void QTAudioPlayer::SetPause(bool bPause)
{
	m_bIsPause = bPause;
	if (!output)
		return;

	if (m_bIsPause)
		output->suspend();
	else
		output->resume();
}

bool QTAudioPlayer::IsPause()
{
	return m_bIsPause;
}

void QTAudioPlayer::SetVolumn(int nVolumn)
{
	if (!output)
		return;
	output->setVolume(nVolumn);
}

int QTAudioPlayer::GetFree()
{
	if (!output)
		return 0;
	int free = output->bytesFree();
	return free;
}

long long QTAudioPlayer::GetNoPlayMs()
{
	if (!output)
		return 0;

	long long pts = 0;
	//还未播放的字节数
	double size = output->bufferSize() - output->bytesFree();
	//一秒音频字节大小
	double secSize = m_nSampleSize * (m_nSampleSize / 8) * m_nChannels;

	if (secSize <= 0)
		pts = 0;
	else
		pts = (size / secSize) * 1000;
	return pts;
}
