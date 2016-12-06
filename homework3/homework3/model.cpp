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
}
