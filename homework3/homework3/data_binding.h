#pragma once

#include "std_header.h"

namespace tester1 {

	const static GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	static GLuint VBO, VAO, shaderProgram, fragmentShader, vertexShader;

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