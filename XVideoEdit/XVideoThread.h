#pragma once
#include <QThread>
#include "opencv2/core.hpp"
#include <QMutex>
class XVideoThread :
	public QThread
{
	Q_OBJECT
public:

	static XVideoThread* Get()
	{
		static XVideoThread vt;
		return &vt;
	}

	~XVideoThread();

public:
	//����Ƶ�ļ�
	bool Open(const std::string file);
	void Play();
	void Pause();

	void run();

	// �򿪶�����ƵԴ�ļ�
	bool Open2(const std::string file);

	//������Ƶ ��ʼ������Ƶ
	bool StartSave(const std::string filename, int width = 0, int height = 0);
	//ֹͣ������Ƶ��д����Ƶ֡������
	void StopSave();

	//���ˮӡ
	void SetMark(cv::Mat m) { mutex.lock(); this->mark = m; mutex.unlock(); };

	//���ص�ǰ���ŵ�λ��
	double GetPos();
	double GetVideofps();

	//��ȡ����
	int	GetWidth() { return nWidth; }
	int GetHeight() { return nHeight; }
	int	GetWidth2() { return nWidth2; }
	int GetHeight2() { return nHeight2; }

	//��ת��Ƶ
	bool Seek(double pos);
	bool Seek(int frame);
signals:
		//��ʾԭ��Ƶ1ͼ��
	void ViewImage1(cv::Mat mat);
	void ViewImage2(cv::Mat mat);

	//��ʾ���ɺ��ͼ��
	void ViewDes(cv::Mat mat);
	void SaveEnd();
public:
	std::string src1file;
	std::string desfile;

	int begin = 0;
	int end = 0;

	void SetBegin(double p);
	void SetEnd(double p);
protected:
	QMutex mutex;
	XVideoThread();

	int fps = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nWidth2 = 0;
	int nHeight2 = 0;
	cv::Mat mark;

	//�Ƿ�ʼд��Ƶ
	bool isWrite = false;

	//����
	bool isPlay = false;
};

