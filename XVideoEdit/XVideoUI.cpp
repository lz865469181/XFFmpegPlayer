#include "XVideoUI.h"
#include <QFileDialog>
#include <string>
#include <QMessageBox>
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "XVideoThread.h"
#include "XFilter.h"
#include "XAudio.h"

bool XVideoUI::isSliderPress = false;
XVideoUI::XVideoUI(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);

	qRegisterMetaType<cv::Mat>("cv::Mat");

	//原视频显示信号触发
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage1(cv::Mat)),
		ui.src1,
		SLOT(SetImage(cv::Mat)));
	//原视频显示信号
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage2(cv::Mat)),
		ui.src2,
		SLOT(SetImage(cv::Mat))
	);

	//输出视频显示信号
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewDes(cv::Mat)),
		ui.des,
		SLOT(SetImage(cv::Mat)));

	//导出视频结束
	QObject::connect(XVideoThread::Get(),
		SIGNAL(SaveEnd()), 
		this,
		SLOT(ExportEnd()));

	Pause();
	nTimer = 0;
}

void XVideoUI::timerEvent(QTimerEvent* e)
{
	if (isSliderPress)
		return;

	double pos = XVideoThread::Get()->GetPos();
	ui.playSlider->setValue(pos * 1000);
}

void XVideoUI::Open()
{
	QString strName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择视频文件"));
	if (strName.isEmpty())
		return;

	std::string strFile = strName.toLocal8Bit().data();
	if (!XVideoThread::Get()->Open(strFile))
	{
		QMessageBox::information(this, "error", strName + " open failed!");
		return;
	}

	nTimer = startTimer(40);

	Play();
}

void XVideoUI::Play()
{
	ui.pauseButton->show();
	ui.pauseButton->setGeometry(ui.playButton->geometry());
	XVideoThread::Get()->Play();
	ui.playButton->hide();
}

void XVideoUI::Pause()
{
	ui.playButton->show();
	ui.pauseButton->hide();
	XVideoThread::Get()->Pause();
}

void XVideoUI::SliderPress()
{
	isSliderPress = true;
}

void XVideoUI::SliderRelease()
{
	isSliderPress = false;
}

void XVideoUI::SetPos(int pos)
{
	XVideoThread::Get()->Seek((double)pos / 1000);
}

void XVideoUI::Left(int pos)
{
	XVideoThread::Get()->SetBegin((double)pos / 1000.);
	SetPos(pos);
}

void XVideoUI::Right(int pos)
{
	XVideoThread::Get()->SetEnd((double)pos / 1000.);
}

void XVideoUI::Set()
{
	XFilter::Get()->Clear();
	//视频图像裁剪
	bool isClip = false;
	double cx = ui.cx->value();
	double cy = ui.cy->value();
	double cw = ui.cw->value();
	double ch = ui.ch->value();
	if (cx + cy + cw + ch > 0.0001)
	{
		isClip = true;
		XFilter::Get()->Add(XTask{ XTASK_CLIP,
		{ cx,cy,cw,ch } });
		double w = XVideoThread::Get()->GetWidth();
		double h = XVideoThread::Get()->GetHeight();
		XFilter::Get()->Add(XTask{ XTASK_RESIZE, { w, h } });
	}

	//图像金字塔
	bool isPy = false;
	int up = ui.pyup->value();
	int down = ui.pydown->value();
	if (up > 0)
	{
		isPy = true;
		XFilter::Get()->Add(XTask{ XTASK_PYUP, {(double)up} });
		int w = XVideoThread::Get()->GetWidth();
		int h = XVideoThread::Get()->GetHeight();

		for (int i = 0; i < up; i++)
		{
			h = h * 2;
			w = w * 2;
		}
		ui.width->setValue(w);
		ui.height->setValue(h);
	}
	if (down > 0)
	{
		isPy = true;
		XFilter::Get()->Add(XTask{ XTASK_PYUP, {(double)down} });
		int w = XVideoThread::Get()->GetWidth();
		int h = XVideoThread::Get()->GetHeight();
		for (int i = 0; i < up; i++)
		{
			h = h / 2;
			w = w / 2;
		}
		ui.width->setValue(w);
		ui.height->setValue(h);
	}
	//调整视频尺寸
	double w = ui.width->value();
	double h = ui.height->value();
	if (ui.width->value() > 0 && ui.height->value() > 0)
	{
		XFilter::Get()->Add(XTask{ XTASK_RESIZE, {w, h} });
	}

	//对比度和亮度
	if (ui.bright->value() > 0 || ui.contrast->value() > 1.0)
	{
		XFilter::Get()->Add(XTask{ XTASK_GAIN,
			{(double)ui.bright->value(), (double)ui.contrast->value()} });
	}

	if (ui.color->currentIndex() == 1)
	{
		XFilter::Get()->Add(XTask{ XTASK_GRAY });
		isColor = false;
	}

	//图像旋转 0 90 180 270
	if (ui.rotate->currentIndex() == 1)
	{
		XFilter::Get()->Add(XTask{ XTASK_ROTATE90 });
	}
	else if (ui.rotate->currentIndex() == 2)
	{
		XFilter::Get()->Add(XTask{ XTASK_ROTATE180 });
	}
	else if (ui.rotate->currentIndex() == 3)
	{
		XFilter::Get()->Add(XTask{ XTASK_ROTATE270 });
	}

	//图形镜像
	if (ui.flip->currentIndex() == 1)
	{
		XFilter::Get()->Add(XTask{ XTASK_FLIPX });
	}
	else if (ui.flip->currentIndex() == 2)
	{
		XFilter::Get()->Add(XTask{ XTASK_FLIPY });
	}
	else if (ui.flip->currentIndex() == 3)
	{
		XFilter::Get()->Add(XTask{ XTASK_FLIPXY });
	}

	//水印
	if (isMark)
	{
		double x = ui.mx->value();
		double y = ui.my->value();
		double a = ui.ma->value();
		XFilter::Get()->Add(XTask{ XTASK_MARK, {x,y,a} });
	}

	//融合
	if (isBlend)
	{
		double a = ui.ba->value();
		XFilter::Get()->Add(XTask{ XTASK_BLEND, { a } });
	}

	//合并
	if (isMerge)
	{
		double h2 = XVideoThread::Get()->GetWidth2();
		double h1 = XVideoThread::Get()->GetHeight();
		int w = XVideoThread::Get()->GetWidth2() * (h2 / h1);
		XFilter::Get()->Add(XTask{ XTASK_MERGE });
		ui.width->setValue(XVideoThread::Get()->GetWidth() + w);
		ui.height->setValue(h1);
	}
}

void XVideoUI::Export()
{
	if (isExport)
	{
		//如果正在导出
		XVideoThread::Get()->StopSave();
		isExport = false;
		ui.exportButton->setText("Start Export");
		return;
	}

	//开始导出
	QString strName = QFileDialog::getSaveFileName(this, "保存", "out1.avi");
	if (strName.isEmpty())
		return ;

	std::string strFileName = strName.toLocal8Bit().data();
	if (XVideoThread::Get()->StartSave(strFileName))
	{
		isExport = true;
		ui.exportButton->setText("Stop Export");
	}
}

void XVideoUI::ExportEnd()
{
	isExport = false;
	ui.exportButton->setText("Start Export");

	std::string src = XVideoThread::Get()->src1file;
	std::string des = XVideoThread::Get()->desfile;

	//处理音频
	XAudio::Get()->ExportA(src, src + ".mp3");
	std::string tmp = des + ".avi";
	QFile::remove(tmp.c_str());
	QFile::rename(des.c_str(), tmp.c_str());
	XAudio::Get()->Merge(tmp, src + ".mp3", des);
}

void XVideoUI::Mark()
{
	isMark = false;
	isBlend = false;
	QString name = QFileDialog::getOpenFileName(this, "select image:");
	if (name.isEmpty())
		return;

	std::string file = name.toLocal8Bit().data();
	cv::Mat mark = cv::imread(file);
	if (mark.empty())
		return;
	XVideoThread::Get()->SetMark(mark);
		isMark = true;
}

void XVideoUI::Blend()
{
	isMark = false;
	isBlend = false;
	QString name = QFileDialog::getOpenFileName(
		this, "select video:");
	if (name.isEmpty())
	{
		return;
	}
	std::string file = name.toLocal8Bit().data();
	isBlend = XVideoThread::Get()->Open2(file);
}

void XVideoUI::Merge()
{
	isMark = false;
	isBlend = false;
	isMerge = false;
	QString name = QFileDialog::getOpenFileName(
		this, "select video:");
	if (name.isEmpty())
	{
		return;
	}
	std::string file = name.toLocal8Bit().data();
	isMerge = XVideoThread::Get()->Open2(file);
}

