#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include <mutex>
#include "IVideoCall.h"
struct AVFrame;
class XVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions, public IVideoCall
{
	Q_OBJECT
public:
	XVideoWidget(QWidget *parent);
	~XVideoWidget();

	virtual void Init(int nWidth, int nHeight);
	virtual void Repaint(AVFrame* frame);

protected:
	//��ʼ��
	void initializeGL();

	//
	void resizeGL(int w, int h);

	void paintGL();
private:
	//shader����
	QGLShaderProgram program;

	//shader��YUV����
	GLuint unis[3] = { 0 };
	//opengl texture��ַ
	GLuint texs[3] = { 0 };

	unsigned char* data[3] = { 0 };

	int m_nWidth;
	int m_nHeight;

	std::mutex mux;
};

