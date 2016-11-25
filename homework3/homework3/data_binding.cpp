#include "data_binding.h"

namespace tester1
{
	GLuint EBO, VBO, VAO;

	void init() {
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void refresh() {
		auto ptr = shader_namespace::shader_list.find("sample_program");
		if (ptr != shader_namespace::shader_list.end())
			ptr->second.use();

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

namespace tester2
{
	GLuint VBO, VAO;

	void init()
	{
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	void refresh()
	{
		auto ptr = shader_namespace::shader_list.find("tester2");
		if (ptr != shader_namespace::shader_list.end())
			ptr->second.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
}

namespace uni_tester {
	GLuint VAO, VBO;

	void init() {
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void refresh()
	{
		auto ptr = shader_namespace::shader_list.find("uni_test");
		if (ptr != shader_namespace::shader_list.end()) {
			ptr->second.use();
			GLfloat timeValue = (GLfloat)glfwGetTime();
			GLfloat greenValue = (sin(timeValue) / 2) + 0.5f;
			GLint vertexColorLocation = glGetUniformLocation(ptr->second.program, "ourColor");
			glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		}

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
}
