#include "model.h"

namespace model {
	Material::Material(const GLuint _diffuse, const GLuint _specular, const float _shininess)
		: diffuse(_diffuse), specular(_specular), shininess(_shininess) {}

	void Material::Bind(const GLuint program, const char * _diffuse, const char * _specular, const char * _shininess) const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
		glUniform1i(glGetUniformLocation(program, _diffuse), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);
		glUniform1i(glGetUniformLocation(program, _specular), 1);

		glUniform1f(glGetUniformLocation(program, _shininess), shininess);
	}

	Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLuint>& _indices, const std::vector<std::string>& _texture, float _shininess):
		vertices(_vertices), indices(indices), textures_name(_texture), shininess(_shininess)
	{
		auto texture_list = texture::get_texture2D_list();
		for (const auto & i : textures_name) {
			auto ptr = texture_list.find(i);
			if (ptr != texture_list.end()) {
				textures_index.push_back(ptr->second.getTexture());
				textures_type.push_back(ptr->second.getType());
			}
			else
				std::cout << "MESH::MESH::TEXTURE_DO_NOT_EXIST" << std::endl;
		}

		setupMesh();
	}

	void Mesh::Draw(GLuint program) {
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;

		for (size_t i = 0; i < textures_index.size(); ++ i)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
											  // Retrieve texture number (the N in diffuse_textureN)
			char number;
			const std::string &name = textures_type[i];

			if (name == "texture_diffuse")
				number = 48 + (diffuseNr++); // Transfer GLuint to stream
			else if (name == "texture_specular")
				number = 48 + (diffuseNr++); // Transfer GLuint to stream

			// Now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
			// And finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures_index[i]);
		}

		glUniform1f(glGetUniformLocation(program, "shininess"), shininess);

		// Draw mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Always good practice to set everything back to defaults once configured.
		for (size_t i = 0; i < textures_index.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	Mesh::~Mesh() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

	void Mesh::setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}

}
