#include "XAudio.h"
#include <iostream>
#include <QMutex>
using namespace std;
class CXAudio :public XAudio
{
public:
	QMutex mutex;
	/////////////////////////////////////////////////////////
	///导出音频文件
	///@para src string 源文件 (多媒体文件)
	///@para out string 输出的音频文件（MP3）
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
	///合并音视频
	///@para v string 视频文件 (avi)
	///@para a string 音频文件（MP3）
	///@para out string 输出文件 （avi）
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
