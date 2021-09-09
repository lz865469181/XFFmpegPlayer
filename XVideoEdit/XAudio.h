#pragma once
#include <string>
class XAudio
{
public:
	static XAudio* Get();

	/////////////////////////////////////////////////////////
	///������Ƶ�ļ�
	///@para src string Դ�ļ� (��ý���ļ�)
	///@para out string �������Ƶ�ļ���MP3��
	virtual bool ExportA(std::string src, std::string out) = 0;

	/////////////////////////////////////////////////////////
	///�ϲ�����Ƶ
	///@para v string ��Ƶ�ļ� (avi)
	///@para a string ��Ƶ�ļ���MP3��
	///@para out string ����ļ� ��avi��
	virtual bool Merge(std::string v, std::string a, std::string out) = 0;

	XAudio();
	virtual ~XAudio();
};

