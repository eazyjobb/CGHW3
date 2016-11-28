#include "data_binding.h"

namespace coord_tester {
	GLuint VAO, VBO;

	void init() {
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void refresh() {
		const auto ptr = shader::get_shader_list().find("coord_test");
		if (ptr != shader::get_shader_list().end()) {
			ptr->second.use();

			glActiveTexture(GL_TEXTURE0);
			auto texture_ptr = texture::get_texture2D_list().find("container.jpg");
			if (texture_ptr != texture::get_texture2D_list().end()) {
				glBindTexture(GL_TEXTURE_2D, texture_ptr->second.getTexture());
				glUniform1i(glGetUniformLocation(ptr->second.getProgram(), "ourTexture1"), 0);
			}

			glActiveTexture(GL_TEXTURE1);
			texture_ptr = texture::get_texture2D_list().find("awesomeface.png");
			if (texture_ptr != texture::get_texture2D_list().end()) {
				glBindTexture(GL_TEXTURE_2D, texture_ptr->second.getTexture());
				glUniform1i(glGetUniformLocation(ptr->second.getProgram(), "ourTexture2"), 1);
			}

			GLint viewLoc = glGetUniformLocation(ptr->second.getProgram(), "view");
			GLint projLoc = glGetUniformLocation(ptr->second.getProgram(), "projection");

			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(coord::get_current_camera()->second.getView()));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(coord::get_current_camera()->second.getProjection()));
		}

		glBindVertexArray(VAO);
		
		GLint modelLoc = glGetUniformLocation(ptr->second.getProgram(), "model");
		for (GLuint i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = glm::radians(20.0f) * (GLfloat)glfwGetTime() * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void release() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
}
