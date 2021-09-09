#pragma once
#include <string>
class XAudio
{
public:
	static XAudio* Get();

	/////////////////////////////////////////////////////////
	///导出音频文件
	///@para src string 源文件 (多媒体文件)
	///@para out string 输出的音频文件（MP3）
	virtual bool ExportA(std::string src, std::string out) = 0;

	/////////////////////////////////////////////////////////
	///合并音视频
	///@para v string 视频文件 (avi)
	///@para a string 音频文件（MP3）
	///@para out string 输出文件 （avi）
	virtual bool Merge(std::string v, std::string a, std::string out) = 0;

	XAudio();
	virtual ~XAudio();
};

