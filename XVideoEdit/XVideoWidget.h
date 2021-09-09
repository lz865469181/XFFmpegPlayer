#pragma once
#include <QOpenGLWidget>
#include "opencv2/core.hpp"

class XVideoWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	XVideoWidget(QWidget* p);
	virtual ~XVideoWidget();

public:
	void paintEvent(QPaintEvent* e);

public slots:
	//����ˢ��
	void SetImage(cv::Mat mat);
protected:
	QImage img;
};

