#include "light.h"

namespace light {
	DirLight::DirLight(const vec3 & _direction, const vec3 & _ambient, const vec3 & _diffuse, const vec3 & _specular)
		: direction(_direction), ambient(_ambient), diffuse(_diffuse), specular(_specular) {}

	void DirLight::Bind(const GLuint program, const char * _direction, const char * _ambient, const char * _diffuse, const char * _specular) const {
		glUniform3f(glGetUniformLocation(program, _direction), direction.x, direction.y, direction.z);
		glUniform3f(glGetUniformLocation(program, _ambient), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(program, _diffuse), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(program, _specular), specular.x, specular.y, specular.z);
	}

	PointLight::PointLight(const vec3 & _position, const vec3 & _ambient, const vec3 & _diffuse, const vec3 & _specular, const float _constant, const float _linear, const float _quadratic)
		: position(_position), ambient(_ambient), diffuse(_diffuse), specular(_specular), constant(_constant), linear(_linear), quadratic(_quadratic) {}

	void PointLight::Bind(const GLuint program, const char * _position, const char * _ambient, const char * _diffuse, const char * _specular,
		const char * _constant, const char * _linear, const char * _quadratic) const {
		glUniform3f(glGetUniformLocation(program, _position), position.x, position.y, position.z);
		glUniform3f(glGetUniformLocation(program, _ambient), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(program, _diffuse), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(program, _specular), specular.x, specular.y, specular.z);
		glUniform1f(glGetUniformLocation(program, _constant), constant);
		glUniform1f(glGetUniformLocation(program, _linear), linear);
		glUniform1f(glGetUniformLocation(program, _quadratic), quadratic);
	}


	SpotLight::SpotLight(const vec3 & _position, const vec3 & _direction, const vec3 & _ambient, const vec3 & _diffuse, const vec3 & _specular,
		const float _constant, const float _linear, const float _quadratic, const float _cutOff, const float _outerCutOff) 
		: position(_position), direction(_direction), ambient(_ambient), diffuse(_diffuse), specular(_specular),
		constant(_constant), linear(_linear), quadratic(_quadratic), cutOff(_cutOff), outerCutOff(_outerCutOff) {}

	void SpotLight::Bind(const GLuint program, const char * _position, const char * _direction, const char * _ambient, const char * _diffuse, const char * _specular,
		const char * _constant, const char * _linear, const char * _quadratic, const char * _cutOff, const char * _outerCutOff) const {
		glUniform3f(glGetUniformLocation(program, _position), position.x, position.y, position.z);
		glUniform3f(glGetUniformLocation(program, _direction), direction.x, direction.y, direction.z);
		glUniform3f(glGetUniformLocation(program, _ambient), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(program, _diffuse), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(program, _specular), specular.x, specular.y, specular.z);
		glUniform1f(glGetUniformLocation(program, _constant), constant);
		glUniform1f(glGetUniformLocation(program, _linear), linear);
		glUniform1f(glGetUniformLocation(program, _quadratic), quadratic);
		glUniform1f(glGetUniformLocation(program, _cutOff), cutOff);
		glUniform1f(glGetUniformLocation(program, _outerCutOff), outerCutOff);
	}


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
