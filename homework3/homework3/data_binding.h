#pragma once

#include "std_header.h"

namespace tester1 {

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

	static GLuint EBO, VBO, VAO, shaderProgram, fragmentShader, vertexShader;

	const static char * vertexShaderSource = "\n\
		#version 330 core\n\
		\n\
		layout(location = 0) in vec3 position;\n\
		\n\
		void main()\n\
		{\n\
			gl_Position = vec4(position.x, position.y, position.z, 1.0);\n\
		}\n"
	;

	const static char * fragmentShaderSource = "\n\
		#version 330 core\n\
		\n\
		out vec4 color;\n\
		\n\
		void main()\n\
		{\n\
			color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
		}\n"
	;

	void tester1_init();
	void tester1_refresh();
};