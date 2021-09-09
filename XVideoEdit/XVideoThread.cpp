#include "XVideoThread.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "XFilter.h"
using namespace cv;

//һ����ƵԴ
static VideoCapture cap1;
static VideoCapture cap2;
static VideoWriter vw;
static bool isExit = false;

void XVideoThread::SetBegin(double p)
{
	mutex.lock();
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = p * count;
	begin = frame;
	mutex.unlock();
}

void XVideoThread::SetEnd(double p)
{
	mutex.lock();
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = p * count;
	end = frame;
	mutex.unlock();
}

XVideoThread::XVideoThread()
{
	start();
}

XVideoThread::~XVideoThread()
{
	isExit = true;
	wait();
}

bool XVideoThread::Open(const std::string file)
{
	std::cout << "open :" << file << std::endl;
	mutex.lock();
	bool re = cap1.open(file);
	mutex.unlock();
	std::cout << re << std::endl;
	if(!re)
		return re;
	fps = cap1.get(CAP_PROP_FPS); //��ȡ��Ƶfps
	nWidth = cap1.get(CAP_PROP_FRAME_WIDTH);
	nHeight = cap1.get(CAP_PROP_FRAME_HEIGHT);
	if (fps <= 0)
		fps = 25;
	src1file = file;
	return true;
}

void XVideoThread::Play()
{
	isPlay = true;
}

void XVideoThread::Pause()
{
	isPlay = false;
}

void XVideoThread::run()
{
	Mat mat1;
	while (true)
	{
		mutex.lock();
		if (isExit)
		{
			mutex.unlock();
			return;
		}

		if (!cap1.isOpened())
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
		if (!isPlay)
		{
			mutex.unlock();
			msleep(5);
			continue;
		}

		//��ȡһ֡��Ƶ�����벢��ɫת��
		int cur = cap1.get(CAP_PROP_POS_FRAMES);
		if ((end > 0 && cur >= end) || !cap1.read(mat1) || mat1.empty())
		{
			mutex.unlock();

			if (isWrite)
			{
				StopSave();
				SaveEnd();
			}

			msleep(5);
			continue;
		}

		Mat mat2 = mark;
		if (cap2.isOpened())
		{
			cap2.read(mat2);
		}

		//��ʾͼ��
		if (!isWrite)
		{
			ViewImage1(mat1);
			if (!mat2.empty())
				ViewImage2(mat2);
		}

		//ͨ��������������Ƶ
		Mat des = XFilter::Get()->Filter(mat1, Mat());

		//��ʾ���ɺ�ͼ��
		if (!isWrite)
			ViewDes(des);

		mutex.unlock();
		//������Ƶ��fps���ӳ�
		int s = 0;
		s = 1000 / fps;
		//д���ļ�����Ҫ�ӳ�
		if (isWrite)
		{
			s = 1;
			vw.write(des);
		}
		msleep(s);
	}
}

bool XVideoThread::Open2(const std::string file)
{
	Seek(0);
	std::cout << "open2 :" << file << std::endl;
	mutex.lock();
	bool re = cap2.open(file);
	mutex.unlock();
	std::cout << re << std::endl;
	if (!re)
		return re;
	nWidth2 = cap2.get(CAP_PROP_FRAME_WIDTH);
	nHeight2 = cap2.get(CAP_PROP_FRAME_HEIGHT);
	return true;
}

bool XVideoThread::StartSave(const std::string filename, int width /*= 0*/, int height /*= 0*/)
{
	//��ʼ�������Ƶ��Ⱦ��ͣ
	Seek(0);
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	if (width <= 0)
		width = cap1.get(CAP_PROP_FRAME_WIDTH);
	if (height <= 0)
		height = cap1.get(CAP_PROP_FRAME_HEIGHT);

	vw.open(filename, VideoWriter::fourcc('x', '2', '6', '4'),
		this->fps, Size(width, height));

	if (!vw.isOpened())
	{
		mutex.unlock();
		std::cout << "start save failed" << std::endl;
		return false;
	}

	isWrite = true;
	desfile = filename;
	mutex.unlock();
	return true;
}

void XVideoThread::StopSave()
{
	mutex.lock();
	vw.release();
	mutex.unlock();
	isWrite = false;
}

double XVideoThread::GetPos()
{
	double pos = 0.0;
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return pos;
	}

	double count = cap1.get(CAP_PROP_FRAME_COUNT); //��ȡ��֡��
	double cur = cap1.get(CAP_PROP_POS_FRAMES); //��ȡ��ǰ֡λ��
	if (count > 0.001)
		pos = cur / count;
	mutex.unlock();
	return pos;
}

double XVideoThread::GetVideofps()
{
	return fps;
}

bool XVideoThread::Seek(double pos)
{
	//��ȡ���е�֡
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = pos * count;
	return Seek(frame);
}

bool XVideoThread::Seek(int frame)
{
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	int re = cap1.set(CAP_PROP_POS_FRAMES, frame);
	mutex.unlock();
	return re;
}
