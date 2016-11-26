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
		auto ptr = shader::shader_list.find("sample_program");
		if (ptr != shader::shader_list.end())
			ptr->second.use();

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void release() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
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
		auto ptr = shader::shader_list.find("tester2");
		if (ptr != shader::shader_list.end())
			ptr->second.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

	void release() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
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
		auto ptr = shader::shader_list.find("uni_test");
		if (ptr != shader::shader_list.end()) {
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

	void release() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
}

namespace texture_tester {
	GLuint VAO, VBO, EBO;
	float ratio = 0.5;

	void init() {
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void refresh() {
		auto ptr = shader::shader_list.find("texture_test");
		if (ptr != shader::shader_list.end()) {
			ptr->second.use();

			glActiveTexture(GL_TEXTURE0);
			auto texture_ptr = texture::texture2D_list.find("container.jpg");
			if (texture_ptr != texture::texture2D_list.end()) {
				glBindTexture(GL_TEXTURE_2D, texture_ptr->second.texture);
				glUniform1i(glGetUniformLocation(ptr->second.program, "ourTexture1"), 0);
			}

			glActiveTexture(GL_TEXTURE1);
			texture_ptr = texture::texture2D_list.find("awesomeface.png");
			if (texture_ptr != texture::texture2D_list.end()) {
				glBindTexture(GL_TEXTURE_2D, texture_ptr->second.texture);
				glUniform1i(glGetUniformLocation(ptr->second.program, "ourTexture2"), 1);
			}

			glUniform1f(glGetUniformLocation(ptr->second.program,"ratio"), ratio);

			glm::mat4 trans;
			trans = glm::rotate(trans, (GLfloat)(glfwGetTime() * glm::radians(50.0)), glm::vec3(0.0f, 0.0f, 1.0f));
			trans = glm::translate(trans, glm::vec3(cos((GLfloat)glfwGetTime())/3.0f, sin((GLfloat)glfwGetTime())/3.0f, 0.0f));

			GLuint transformLoc = glGetUniformLocation(ptr->second.program, "trans");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		}

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void release() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
}
