#include "XAudio.h"
#include <iostream>
#include <QMutex>
using namespace std;
class CXAudio :public XAudio
{
public:
	QMutex mutex;
	/////////////////////////////////////////////////////////
	///������Ƶ�ļ�
	///@para src string Դ�ļ� (��ý���ļ�)
	///@para out string �������Ƶ�ļ���MP3��
	bool ExportA(std::string src, std::string out)
	{
		//ffmpeg -i test.mp4 -vn -y test.mp3
		string cmd = "ffmpeg ";
		cmd += " -i ";
		cmd += src;
		cmd += " -vn -y ";
		cmd += out;
		cout << cmd << endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}

	/////////////////////////////////////////////////////////
	///�ϲ�����Ƶ
	///@para v string ��Ƶ�ļ� (avi)
	///@para a string ��Ƶ�ļ���MP3��
	///@para out string ����ļ� ��avi��
	bool Merge(std::string v, std::string a, std::string out)
	{
		//ffmpeg -i test.avi -i test.mp3 -c copy out.avi
		string cmd = "ffmpeg -i ";
		cmd += v;
		cmd += " -i ";
		cmd += a;
		cmd += " -c copy ";
		cmd += out;
		cout << cmd << endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}


};

XAudio * XAudio::Get()
{
	static CXAudio ca;
	return &ca;
}
XAudio::XAudio()
{
}


XAudio::~XAudio()
{
}
