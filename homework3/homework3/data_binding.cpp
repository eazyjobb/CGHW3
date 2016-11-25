#include "data_binding.h"

namespace tester1
{
	GLuint EBO, VBO, VAO;

	void tester1_init() {
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

	void tester1_refresh() {
		auto ptr = shader_namespace::shader_list.find("sample_program");
		if (ptr != shader_namespace::shader_list.end())
			ptr->second.use();

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};
