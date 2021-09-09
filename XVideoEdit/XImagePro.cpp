#include "XImagePro.h"
#include <opencv2/imgproc.hpp>
void XImagePro::Set(cv::Mat mat1, cv::Mat mat2)
{
	if (mat1.empty())
		return;

	src1 = mat1;
	src2 = mat2;
	this->src1.copyTo(des);
}

void XImagePro::Gain(double bright, double constrast)
{
	if (des.empty())
		return;
	des.convertTo(des, -1, constrast, bright);
}

void XImagePro::Rotate90()
{
	if (des.empty())
		return;
	rotate(des, des, cv::ROTATE_90_CLOCKWISE);
}

void XImagePro::Rotate180()
{
	if (des.empty())
		return;
	rotate(des, des, cv::ROTATE_180);
}

void XImagePro::Rotate270()
{
	if (des.empty())
		return;
	rotate(des, des, cv::ROTATE_90_COUNTERCLOCKWISE);
}

void XImagePro::FlipX()
{
	if (des.empty())
		return;
	flip(des, des, 0);
}

void XImagePro::FlipY()
{
	if (des.empty())
		return;
	flip(des, des, 1);
}

void XImagePro::FlipXY()
{
	if (des.empty())
		return;
	flip(des, des, -1);
}

void XImagePro::Resize(int width, int height)
{
	if (des.empty())
		return;
	resize(des, des, cv::Size(width, height));
}

void XImagePro::PyDown(int count)
{
	if (des.empty())
		return;
	for (int i = 0; i < count; i++)
		pyrDown(des, des);
}

void XImagePro::PyUp(int count)
{
	if (des.empty())
		return;
	for (int i = 0; i < count; i++)
		pyrUp(des, des);
}

void XImagePro::Clip(int x, int y, int w, int h)
{
	if (des.empty())
		return;
	if (x < 0 || y < 0 || w < 0 || h < 0)
		return;

	if (x > des.cols || y > des.rows)return;
		des = des(cv::Rect(x, y, w, h));
}

void XImagePro::Gray()
{
	if (des.empty())
		return;
	cv::cvtColor(des, des, cv::COLOR_BGR2GRAY);
}

void XImagePro::Mark(int x, int y, double a)
{
	if (des.empty())return;
	if (src2.empty())return;
	cv::Mat rol = des(cv::Rect(x, y, src2.cols, src2.rows));
	addWeighted(src2, a, rol, 1 - a, 0, rol);
}

void XImagePro::Blend(double a)
{
	if (des.empty())return;
	if (src2.empty())return;
	if (src2.size() != des.size())
	{
		resize(src2, des, des.size());
	}
	addWeighted(src2, a, des, 1 - a, 0, des);
}

void XImagePro::Merge()
{
	if (des.empty())
		return;
	if (src2.empty())
		return;
	if (src2.size() != des.size())
	{
		int w = src2.cols * ((double)src2.rows / (double)des.rows);
		resize(src2, src2, cv::Size(w, des.rows));
	}
	int dw = des.cols + src2.cols;
	int dh = des.rows;
	des = cv::Mat(cv::Size(dw, dh), src1.type());
	cv::Mat r1 = des(cv::Rect(0, 0, src1.cols, dh));
	cv::Mat r2 = des(cv::Rect(src1.cols, 0, src2.cols, dh));

	src1.copyTo(r1);
	src2.copyTo(r2);
}

XImagePro::XImagePro()
{

}

XImagePro::~XImagePro()
{

}
