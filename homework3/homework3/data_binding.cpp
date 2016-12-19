#include "data_binding.h"

namespace coord_tester {
	const static GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	const static glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

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

namespace light_tester {
	GLuint VAO, VBO;

	GLfloat vertices[] = {
		// Positions          // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	void init() {
		glm::vec3 amb, dif, spc;
		amb = glm::vec3(0.2f, 0.2f, 0.2f);
		dif = glm::vec3(0.5f, 0.5f, 0.5f);
		spc = glm::vec3(1.0f, 1.0f, 1.0f);

		light::insertPointLight("pointLight",lightPos, amb, dif, spc, 1.0f, 0.14f, 0.07f);

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void refresh() {
		glm::mat4 model;
		model::Material box(
			texture::get_texture2D_list().find("container2.png")->second.getTexture(),
			texture::get_texture2D_list().find("container2_specular.png")->second.getTexture(),
			32.0f
		);

		const light::DirLight & dirliggt = light::getSunLight();

		glBindVertexArray(VAO);
		
			auto ptr = shader::get_shader_list().find("new_light");	//箱子
			
			if (ptr != shader::get_shader_list().end()) {
				ptr->second.use();
				auto program = ptr->second.getProgram();

				box.Bind(program, "material.diffuse", "material.specular", "material.shininess");
				dirliggt.Bind(program, "dirLight.direction", "dirLight.ambient", "dirLight.diffuse", "dirLight.specular");

				glUniform1i(glGetUniformLocation(program, "num_of_point_lights"), light::getPointLightNum());

				char s[7][30] = {
					"pointLights[0].position", "pointLights[0].ambient", "pointLights[0].diffuse",
					"pointLights[0].specular", "pointLights[0].constant", "pointLights[0].linear",
					"pointLights[0].quadratic"
				};

				const auto & pll = light::getPointList();
				for (const auto & i : pll) {
					i.second.Bind(program, s[0], s[1], s[2], s[3], s[4], s[5], s[6]);
					for (int j = 0; j < 7; ++j)
						++ s[j][12];
				}

				coord::get_current_camera()->second.Bind(program,"view","projection","viewPos");
			}

			GLint modelLoc = glGetUniformLocation(ptr->second.getProgram(), "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			
			//////////////////点光源
			ptr = shader::get_shader_list().find("lamb"); 
			
			if (ptr != shader::get_shader_list().end()) {
				ptr->second.use();

				auto program = ptr->second.getProgram();
				coord::get_current_camera()->second.Bind(program, "view", "projection", "viewPos");
			}
			
			modelLoc = glGetUniformLocation(ptr->second.getProgram(), "model");

			model = glm::mat4();
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
	}

	void release() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
}

namespace model_tester {
	//model::Model mod("nanosuit.obj");
	GLuint VAO, VBO;

	GLfloat vertices[] = {
		// Positions          // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	void init() {

		glm::vec3 amb, dif, spc;
		amb = glm::vec3(0.2f, 0.2f, 0.2f);
		dif = glm::vec3(0.5f, 0.5f, 0.5f);
		spc = glm::vec3(1.0f, 1.0f, 1.0f);

		light::insertPointLight("pointLight", lightPos, amb, dif, spc, 1.0f, 0.14f, 0.07f);

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		glBindVertexArray(0);

		
		std::cerr << "Hello! Now in the model_tester!" << std::endl;
		if (!model::read_model("nanosuit.obj")) {
			std::cerr << "model_tester::init:: read_model failed" << std::endl;
		}
		else {
			std::cerr << "loading obj successfully" << std::endl;
		}

		if (!model::read_model("miku.pmd")) {
			std::cerr << "model_tester::init:: read_model failed" << std::endl;
		}
		else {
			std::cerr << "loading obj successfully" << std::endl;
		}
	}

	void refresh() {
		
		glm::mat4 model;
		model::Material box(
			texture::get_texture2D_list().find("container2.png")->second.getTexture(),
			texture::get_texture2D_list().find("container2_specular.png")->second.getTexture(),
			32.0f
		);
		
		const light::DirLight & dirliggt = light::getSunLight();

		glBindVertexArray(VAO);
		
			auto ptr = shader::get_shader_list().find("new_light");	//箱子
																	
			if (ptr != shader::get_shader_list().end()) {
				ptr->second.use();
				auto program = ptr->second.getProgram();

				box.Bind(program, "material.diffuse", "material.specular", "material.shininess");	//不能去掉，绑定了漫反射系数
				dirliggt.Bind(program, "dirLight.direction", "dirLight.ambient", "dirLight.diffuse", "dirLight.specular");

				glUniform1i(glGetUniformLocation(program, "num_of_point_lights"), light::getPointLightNum());

				char s[7][30] = {
					"pointLights[0].position", "pointLights[0].ambient", "pointLights[0].diffuse",
					"pointLights[0].specular", "pointLights[0].constant", "pointLights[0].linear",
					"pointLights[0].quadratic"
				};

				const auto & pll = light::getPointList();
				for (const auto & i : pll) {
					i.second.Bind(program, s[0], s[1], s[2], s[3], s[4], s[5], s[6]);
					for (int j = 0; j < 7; ++j)
						++s[j][12];
				}

				coord::get_current_camera()->second.Bind(program, "view", "projection", "viewPos");
			}

		
			//////////////////点光源
			ptr = shader::get_shader_list().find("lamb");

			if (ptr != shader::get_shader_list().end()) {
				ptr->second.use();

				auto program = ptr->second.getProgram();
				coord::get_current_camera()->second.Bind(program, "view", "projection", "viewPos");
			}
			GLuint			
			modelLoc = glGetUniformLocation(ptr->second.getProgram(), "model");

			model = glm::mat4();
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		

		glBindVertexArray(0);
		
		ptr = shader::get_shader_list().find("new_light");

		if (ptr != shader::get_shader_list().end()) {
			ptr->second.use();
			auto program = ptr->second.getProgram();
			coord::get_current_camera()->second.Bind(program, "view", "projection", "viewPos");
		}

		modelLoc = glGetUniformLocation(ptr->second.getProgram(), "model");
		
		int ang = 0;

		const auto &mp = model::getModelList();
		for (const auto &i : mp) {

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(ang, ang, ang));
			model = glm::scale(model, glm::vec3(0.2f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			i.second.Draw(ptr->second.getProgram());

			ang += 2;
		}
		
	}

	void release() {
		model::release_all_model();
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
}
