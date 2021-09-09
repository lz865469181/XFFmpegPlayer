#pragma once
class XAudioPlayer
{
public:
	//����Ƶ�����豸
	virtual bool Open() = 0;
	virtual void Close() = 0;
	//��ղ����豸�Ļ�������
	virtual void Clear() = 0;

	//д����Ƶ���Ż���
	virtual bool WriteBuf(const unsigned char *buf, int dataSize) = 0;

	//������Ƶ��ͣ
	virtual void SetPause(bool bPause) = 0;
	virtual bool IsPause() = 0;

	//����������С
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
	//����Ƶ�����豸
	virtual bool Open();
	virtual void Close();
	//��ղ����豸�Ļ�������
	virtual void Clear();

	//д����Ƶ���Ż���
	virtual bool WriteBuf(const unsigned char *buf, int dataSize);

	//������Ƶ��ͣ
	virtual void SetPause(bool bPause);
	virtual bool IsPause();

	//����������С
	virtual void SetVolumn(int nVolumn);

	//��ȡ��ǰ
	virtual int GetFree();
	//���ػ����л�û�в��ŵ�ʱ�䣨���룩
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

