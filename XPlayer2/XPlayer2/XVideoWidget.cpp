#include "XVideoWidget.h"
#include <QTimer>
#include <iostream>
extern "C"
{
#include <libavutil/frame.h>
}

#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4 

//����shader
const char* vertexStr = GET_STR(
	attribute vec4 vertexIn;
attribute vec2 textureIn;
varying vec2 textureOut;
void main(void)
{
	gl_Position = vertexIn;
	textureOut = textureIn;
}
);

//ƬԪshader
const char* tString = GET_STR(
	varying vec2 textureOut;
uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;

void main(void)
{
	vec3 yuv;
	vec3 rgb;
	yuv.x = texture2D(tex_y, textureOut).r;
	yuv.y = texture2D(tex_u, textureOut).r - 0.5;
	yuv.z = texture2D(tex_v, textureOut).r - 0.5;
	rgb = mat3(1.0, 1.0, 1.0, 
		0.0, -0.39465, 2.03211, 
		1.13983, -0.58060, 0.0) * yuv;
	gl_FragColor = vec4(rgb, 1.0);
}
);

XVideoWidget::XVideoWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
}

XVideoWidget::~XVideoWidget()
{
}

void XVideoWidget::Init(int nWidth, int nHeight)
{
	mux.lock();
	this->m_nWidth = nWidth;
	this->m_nHeight = nHeight;

	delete data[0];
	delete data[1];
	delete data[2];

	//��������ڴ�ռ�
	data[0] = new unsigned char[nWidth * nHeight]; //Y
	data[1] = new unsigned char[nWidth * nHeight / 4]; //U
	data[2] = new unsigned char[nWidth * nHeight / 4]; //V

	std::cout << glGetString(GL_VERSION) << std::endl;

	//��ղ���
	if (texs[0])
	{
		glDeleteTextures(3, texs);
	}
	//��������
	glGenTextures(3, texs);

	//Y
	glBindTexture(GL_TEXTURE_2D, texs[0]);
	//�Ŵ���ˣ����Բ�ֵ, GL_NEAREST(Ч�ʸ�)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, nWidth, nHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	//U
	glBindTexture(GL_TEXTURE_2D, texs[1]);
	//�Ŵ���ˣ����Բ�ֵ, GL_NEAREST(Ч�ʸ�)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, nWidth / 2, nHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	//V
	glBindTexture(GL_TEXTURE_2D, texs[2]);
	//�Ŵ���ˣ����Բ�ֵ, GL_NEAREST(Ч�ʸ�)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, nWidth / 2, nHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	mux.unlock();
}

void XVideoWidget::Repaint(AVFrame* frame)
{
	if (!frame)
		return;
	//�ж�������
	mux.lock();
	if (!data[0] || m_nWidth*m_nHeight == 0 || frame->width != this->m_nWidth || frame->height != m_nHeight)
	{
		av_frame_free(&frame);
		mux.unlock();
		return;
	}

	if (m_nWidth == frame->linesize[0]) //�������
	{
		memcpy(data[0], frame->data[0], m_nWidth* m_nHeight);
		memcpy(data[1], frame->data[1], m_nWidth* m_nHeight / 4);
		memcpy(data[2], frame->data[2], m_nWidth* m_nHeight / 4);
	}
	else //�ж�������
	{
		for(int i = 0; i<m_nHeight; i++) // Y
			memcpy(data[0] + m_nWidth * i, frame->data[0] + frame->linesize[0]*i, m_nWidth);
		for (int i = 0; i < m_nHeight/2; i++) // U
			memcpy(data[1] + m_nWidth/2 * i, frame->data[1] + frame->linesize[1] * i, m_nWidth);
		for (int i = 0; i < m_nHeight / 2; i++) // V
			memcpy(data[2] + m_nWidth / 2 * i, frame->data[2] + frame->linesize[2] * i, m_nWidth);
	}
	mux.unlock();
	av_frame_free(&frame);

	//ˢ����ʾ
	update();
}

void XVideoWidget::initializeGL()
{
	mux.lock();
	//��ʼ��opengl OpenGLFunctions �̳� ����
	initializeOpenGLFunctions();

	//program ����shader�ű������ƬԪ
	// ƬԪshader
	program.addShaderFromSourceCode(QGLShader::Fragment, tString);
	//����shader
	program.addShaderFromSourceCode(QGLShader::Vertex, vertexStr);

	//���ö�������ı���
	program.bindAttributeLocation("vertexIn", A_VER);
	//��������
	program.bindAttributeLocation("textureIn", T_VER);

	//����shader
	program.link();
	program.bind();

	//���ݶ���Ͳ�������
	static const GLfloat ver[] = {
		-1.0f,-1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f,
	};

	static const GLfloat tex[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};

	//����
	glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
	glEnableVertexAttribArray(A_VER);

	//����
	glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
	glEnableVertexAttribArray(T_VER);  //��ֵ��Ҫ ������ʾ������

	//��shader��ȡ����
	unis[0] = program.uniformLocation("tex_y");
	unis[1] = program.uniformLocation("tex_u");
	unis[2] = program.uniformLocation("tex_v");

	mux.unlock();
}

void XVideoWidget::resizeGL(int w, int h)
{

}

void XVideoWidget::paintGL()
{
	mux.lock();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texs[0]); // 0��󶨵�Y����
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nWidth, m_nHeight, GL_RED, GL_UNSIGNED_BYTE, data[0]);
	glUniform1i(unis[0], 0); //�� shader uni ��������

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texs[1]); // 1��󶨵�Y����
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nWidth / 2, m_nHeight / 2, GL_RED, GL_UNSIGNED_BYTE, data[1]);
	glUniform1i(unis[1], 1); //�� shader uni ��������

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texs[2]); // 1��󶨵�Y����
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nWidth / 2, m_nHeight / 2, GL_RED, GL_UNSIGNED_BYTE, data[2]);
	glUniform1i(unis[2], 2); //�� shader uni ��������

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	mux.unlock();
}
