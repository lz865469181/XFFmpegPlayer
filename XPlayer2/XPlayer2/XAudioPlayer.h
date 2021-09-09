#pragma once
class XAudioPlayer
{
public:
	//打开音频播放设备
	virtual bool Open() = 0;
	virtual void Close() = 0;
	//清空播放设备的缓存数据
	virtual void Clear() = 0;

	//写入音频播放缓存
	virtual bool WriteBuf(const unsigned char *buf, int dataSize) = 0;

	//设置音频暂停
	virtual void SetPause(bool bPause) = 0;
	virtual bool IsPause() = 0;

	//设置音量大小
	virtual void SetVolumn(int nVolumn) = 0;
	virtual void SetSampleRate(int nSampleRate) = 0;
	virtual void SetSampleSize(int nSampleSize) = 0;
	virtual void SetChannel(int nChannel) = 0;

	static XAudioPlayer* Get();
public:
	XAudioPlayer();
	~XAudioPlayer();
};

class QAudioOutput;
class QIODevice;
class QTAudioPlayer : public XAudioPlayer
{
public:
	QTAudioPlayer();
	~QTAudioPlayer();
public:
	//打开音频播放设备
	virtual bool Open();
	virtual void Close();
	//清空播放设备的缓存数据
	virtual void Clear();

	//写入音频播放缓存
	virtual bool WriteBuf(const unsigned char *buf, int dataSize);

	//设置音频暂停
	virtual void SetPause(bool bPause);
	virtual bool IsPause();

	//设置音量大小
	virtual void SetVolumn(int nVolumn);

	//获取当前
	virtual int GetFree();
	//返回缓冲中还没有播放的时间（毫秒）
	virtual long long GetNoPlayMs();

	void SetSampleRate(int nSampleRate)
	{
		m_nSampleRate = nSampleRate;
	}

	void SetSampleSize(int nSampleSize)
	{
		m_nSampleSize = nSampleSize;
	}

	void SetChannel(int nChannel)
	{
		m_nChannels = nChannel;
	}
private:
	int m_nSampleRate;
	int m_nSampleSize;
	int m_nChannels;

	QAudioOutput* output;
	QIODevice *io;

	bool m_bIsPause;
};

