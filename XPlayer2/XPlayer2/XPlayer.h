#pragma once

#include <QtWidgets/QWidget>
#include "ui_XPlayer.h"
class XPlayer : public QWidget
{
    Q_OBJECT

public:
    XPlayer(QWidget *parent = Q_NULLPTR);
	~XPlayer();

	//��ʱ�� ��������ʾ
	void timerEvent(QTimerEvent *e);
	//���ڳߴ�仯
	void resizeEvent(QResizeEvent *e);

	//˫��ȫ��
	void mouseDoubleClickEvent(QMouseEvent *e);

	void SetPause(bool isPause);

public slots:
	void OpenFile();
	void PlayOrPause();

	//����slider�������϶��¼�
	void SliderPress();
	void SliderRelease();

private:
	bool isSliderPress = false;
    Ui::XPlayerClass ui;
};
