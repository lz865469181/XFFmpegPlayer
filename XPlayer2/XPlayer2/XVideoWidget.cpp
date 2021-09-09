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

//顶点shader
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

//片元shader
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

	//分配材质内存空间
	data[0] = new unsigned char[nWidth * nHeight]; //Y
	data[1] = new unsigned char[nWidth * nHeight / 4]; //U
	data[2] = new unsigned char[nWidth * nHeight / 4]; //V

	std::cout << glGetString(GL_VERSION) << std::endl;

	//清空材质
	if (texs[0])
	{
		glDeleteTextures(3, texs);
	}
	//创建材质
	glGenTextures(3, texs);

	//Y
	glBindTexture(GL_TEXTURE_2D, texs[0]);
	//放大过滤，线性差值, GL_NEAREST(效率高)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//创建材质显卡空间
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, nWidth, nHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	//U
	glBindTexture(GL_TEXTURE_2D, texs[1]);
	//放大过滤，线性差值, GL_NEAREST(效率高)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//创建材质显卡空间
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, nWidth / 2, nHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	//V
	glBindTexture(GL_TEXTURE_2D, texs[2]);
	//放大过滤，线性差值, GL_NEAREST(效率高)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//创建材质显卡空间
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, nWidth / 2, nHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	mux.unlock();
}

void XVideoWidget::Repaint(AVFrame* frame)
{
	if (!frame)
		return;
	//行对齐问题
	mux.lock();
	if (!data[0] || m_nWidth*m_nHeight == 0 || frame->width != this->m_nWidth || frame->height != m_nHeight)
	{
		av_frame_free(&frame);
		mux.unlock();
		return;
	}

	if (m_nWidth == frame->linesize[0]) //无需对齐
	{
		memcpy(data[0], frame->data[0], m_nWidth* m_nHeight);
		memcpy(data[1], frame->data[1], m_nWidth* m_nHeight / 4);
		memcpy(data[2], frame->data[2], m_nWidth* m_nHeight / 4);
	}
	else //行对齐问题
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

	//刷新显示
	update();
}

void XVideoWidget::initializeGL()
{
	mux.lock();
	//初始化opengl OpenGLFunctions 继承 函数
	initializeOpenGLFunctions();

	//program 加载shader脚本顶点和片元
	// 片元shader
	program.addShaderFromSourceCode(QGLShader::Fragment, tString);
	//顶点shader
	program.addShaderFromSourceCode(QGLShader::Vertex, vertexStr);

	//设置顶点坐标的变量
	program.bindAttributeLocation("vertexIn", A_VER);
	//材质坐标
	program.bindAttributeLocation("textureIn", T_VER);

	//编译shader
	program.link();
	program.bind();

	//传递顶点和材质坐标
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

	//顶点
	glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
	glEnableVertexAttribArray(A_VER);

	//材质
	glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
	glEnableVertexAttribArray(T_VER);  //数值重要 传错显示不出来

	//从shader获取材质
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
	glBindTexture(GL_TEXTURE_2D, texs[0]); // 0层绑定到Y材质
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nWidth, m_nHeight, GL_RED, GL_UNSIGNED_BYTE, data[0]);
	glUniform1i(unis[0], 0); //与 shader uni 遍历关联

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texs[1]); // 1层绑定到Y材质
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nWidth / 2, m_nHeight / 2, GL_RED, GL_UNSIGNED_BYTE, data[1]);
	glUniform1i(unis[1], 1); //与 shader uni 遍历关联

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texs[2]); // 1层绑定到Y材质
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nWidth / 2, m_nHeight / 2, GL_RED, GL_UNSIGNED_BYTE, data[2]);
	glUniform1i(unis[2], 2); //与 shader uni 遍历关联

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	mux.unlock();
}
