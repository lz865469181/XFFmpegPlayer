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
	//打开视频文件
	bool Open(const std::string file);
	void Play();
	void Pause();

	void run();

	// 打开二号视频源文件
	bool Open2(const std::string file);

	//保存视频 开始保存视频
	bool StartSave(const std::string filename, int width = 0, int height = 0);
	//停止保存视频，写入视频帧的索引
	void StopSave();

	//添加水印
	void SetMark(cv::Mat m) { mutex.lock(); this->mark = m; mutex.unlock(); };

	//返回当前播放的位置
	double GetPos();
	double GetVideofps();

	//获取长宽
	int	GetWidth() { return nWidth; }
	int GetHeight() { return nHeight; }
	int	GetWidth2() { return nWidth2; }
	int GetHeight2() { return nHeight2; }

	//跳转视频
	bool Seek(double pos);
	bool Seek(int frame);
signals:
		//显示原视频1图形
	void ViewImage1(cv::Mat mat);
	void ViewImage2(cv::Mat mat);

	//显示生成后的图形
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

	//是否开始写视频
	bool isWrite = false;

	//播放
	bool isPlay = false;
};

