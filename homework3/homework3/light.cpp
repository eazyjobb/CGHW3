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

	vec3 sun_dir, sun_amb, sun_dif, sun_spc;
	int point_light_num;
	std::unordered_map <std::string, PointLight> point_list;

	const DirLight & getSunLight() {
		static DirLight sunLight(sun_dir, sun_amb, sun_dif, sun_spc);
		return sunLight;
	}

	const int getPointLightNum() {
		return point_light_num;
	}

	bool insertPointLight(const char * name, const vec3 & _position, const vec3 & _ambient, const vec3 & _diffuse, const vec3 & _specular, const float _constant, const float _linear, const float _quadratic)
	{
		if (point_light_num >=4 || point_list.count(name))
			return false;
		++ point_light_num;
		point_list.insert(std::make_pair(name, PointLight(_position, _ambient, _diffuse, _specular, _constant, _linear, _quadratic)));
		return true;
	}

	bool deletaPointLight(const char * name)
	{
		if (point_list.count(name) == false)
			return false;
		-- point_light_num;
		point_list.erase(name);
		return true;
	}

	const std::unordered_map<std::string, PointLight>& getPointList()
	{
		return point_list;
	}

	int init() {
		point_light_num = 0;
		sun_dir = vec3(0.0f, -1.0f, 0.0f);
		sun_amb = vec3(0.2f, 0.2f, 0.2f);
		sun_dif = vec3(0.5f, 0.5f, 0.5f);
		sun_spc = vec3(1.0f, 1.0f, 1.0f);
		return 0;
	}
}
