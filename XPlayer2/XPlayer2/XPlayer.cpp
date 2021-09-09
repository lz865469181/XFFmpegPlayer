#include "XPlayer.h"
#include <QFileDialog>
#include "XDemuxThread.h"
#include <QMessageBox>

static XDemuxThread demux;
XPlayer::XPlayer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	demux.Start();

	startTimer(40);
}

XPlayer::~XPlayer()
{
	demux.Stop();
}

void XPlayer::timerEvent(QTimerEvent *e)
{
	if (isSliderPress)
		return;

	long long total = demux.totalMs;
	if (total > 0)
	{
		double pos = (double)demux.pts / (double)total;
		int v = ui.horizontalSlider->maximum() * pos;
		ui.horizontalSlider->setValue(v);
	}
}

void XPlayer::resizeEvent(QResizeEvent *e)
{
	ui.horizontalSlider->move(50, this->height() - 100);
	ui.horizontalSlider->resize(this->width() - 100, ui.horizontalSlider->height());
	ui.openfile->move(100, this->height() - 150);
	ui.playbtn->move(ui.openfile->x() + ui.openfile->width() + 10, ui.openfile->y());
	ui.video->resize(this->size());
}

void XPlayer::mouseDoubleClickEvent(QMouseEvent *e)
{
	if (isFullScreen())
		this->showNormal();
	else
		this->showFullScreen();
}

void XPlayer::SetPause(bool isPause)
{
	if (isPause)
		ui.playbtn->setText(QString::fromLocal8Bit("播 放"));
	else
		ui.playbtn->setText(QString::fromLocal8Bit("暂 停"));
}

void XPlayer::OpenFile()
{
	//选择文件路径
	QString strFile = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择视频文件"));
	if (strFile.isEmpty())
		return;

	if (!demux.Open(strFile.toLocal8Bit(), ui.video))
	{
		QMessageBox::information(0, "error", "open file failed");
		return;
	}

	SetPause(demux.isPause);
}

void XPlayer::PlayOrPause()
{
	bool isPause = !demux.isPause;
	SetPause(isPause);
	demux.SetPause(isPause);
}

void XPlayer::SliderPress()
{
	isSliderPress = true;
}

void XPlayer::SliderRelease()
{
	isSliderPress = false;
	double pos = 0.0;
	pos = (double)ui.horizontalSlider->value() / (double)ui.horizontalSlider->maximum();
	demux.Seek(pos);
}
