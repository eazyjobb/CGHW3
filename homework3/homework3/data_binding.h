#pragma once

#include "std_header.h"
#include "shader.h"

namespace tester1
{
	const static GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};

	const static GLuint indices[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	extern GLuint EBO, VBO, VAO;

	void init();
	void refresh();
}

namespace tester2
{
	const static GLfloat vertices[] = {
		// 位置              // 颜色
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};

	extern GLuint VBO, VAO;
	void init();
	void refresh();
}

namespace uni_tester
{
	const static GLfloat vertices[] = {
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};

	extern GLuint VBO, VAO;
	void init();
	void refresh();
}