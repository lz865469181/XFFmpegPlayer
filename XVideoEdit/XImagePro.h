#pragma once
#include <opencv2/core.hpp>
class XImagePro
{
public:
	//����ԭͼ������������
	void Set(cv::Mat mat1, cv::Mat mat2);

	//��ȡ������
	cv::Mat Get() { return des; }

	//���öԱȶȺ�����
	void Gain(double bright, double constrast);

	//��ת
	void Rotate90();
	void Rotate180();
	void Rotate270();

	//ͼ����
	void FlipX();
	void FlipY();
	void FlipXY();

	//ͼ��ߴ�
	void Resize(int width, int height);

	//ͼ�������
	void PyDown(int count);
	void PyUp(int count);

	//ͼ��ü�
	void Clip(int x, int y, int w, int h);

	//תΪ�Ҷ�ͼ
	void Gray();

	//ˮӡ
	void Mark(int x, int y, double a);

	//�ں�
	void Blend(double a);

	//�ϲ�
	void Merge();

	XImagePro();
	~XImagePro();

private:
	//ԭͼ
	cv::Mat src1, src2;

	//Ŀ��ͼ
	cv::Mat des;
};

