#pragma once
#include "std_header.h"

namespace light {
	using glm::vec3;
	//平行光
	class DirLight {
	private:
		vec3 direction;		//方向
		vec3 ambient;		//全局光照
		vec3 diffuse;		//漫反射
		vec3 specular;		//镜面
	public:
		DirLight(const vec3 &_direction, const vec3 &_ambient, const vec3 &_diffuse, const vec3 &_specular);
		void Bind(const GLuint program, const char* _direction, const char* _ambient, const char* _diffuse, const char* _specular) const;
	};
	//点光源
	class PointLight {
	private:
		vec3 position;		//光照位置
		float constant;		//衰减系数
		float linear;		//衰减系数
		float quadratic;	//衰减系数
		vec3 ambient;		//全局光照
		vec3 diffuse;		//漫反射
		vec3 specular;		//镜面
	public:
		PointLight(const vec3 &_position, const vec3 &_ambient, const vec3 &_diffuse, const vec3 &_specular,
			const float _constant, const float _linear, const float _quadratic);
		void Bind(const GLuint program, const char* _position, const char* _ambient, const char* _diffuse, const char* _specular,
			const char* _constant, const char* _linear, const char* _quadratic) const;
	};
	//聚焦光
	class SpotLight {
	private:
		vec3 position;		//光照位置
		vec3 direction;		//光照方向
		float cutOff;		//清晰的内圈大小
		float outerCutOff;	//模糊的外圈大小
		float constant;		//衰减系数
		float linear;		//衰减系数
		float quadratic;	//衰减系数
		vec3 ambient;		//全局光照
		vec3 diffuse;		//漫反射
		vec3 specular;		//镜面
	public:
		SpotLight(const vec3 &_position, const vec3 &_direction, const vec3 &_ambient, const vec3 &_diffuse, const vec3 &_specular,
			const float _constant, const float _linear, const float _quadratic, const float _cutOff, const float _outerCutOff);
		void Bind(const GLuint program, const char* _position, const char* _direction, const char* _ambient, const char* _diffuse, const char* _specular,
			const char* _constant, const char* _linear, const char* _quadratic, const char* _cutOff, const char* _outerCutOff) const;
	};

	const DirLight & getSunLight();
	const int getPointLightNum();
	bool insertPointLight(const char * name, const vec3 &_position, const vec3 &_ambient, const vec3 &_diffuse, const vec3 &_specular,
		const float _constant, const float _linear, const float _quadratic);
	bool deletaPointLight(const char * name);
	const std::unordered_map <std::string, PointLight> & getPointList();

	int init();
}
