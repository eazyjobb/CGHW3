#pragma once
#include "std_header.h"

namespace light {
	using glm::vec3;
	//ƽ�й�
	class DirLight {
	private:
		vec3 direction;		//����
		vec3 ambient;		//ȫ�ֹ���
		vec3 diffuse;		//������
		vec3 specular;		//����
	public:
		DirLight(const vec3 &_direction, const vec3 &_ambient, const vec3 &_diffuse, const vec3 &_specular);
		void Bind(const GLuint program, const char* _direction, const char* _ambient, const char* _diffuse, const char* _specular) const;
	};
	//���Դ
	class PointLight {
	private:
		vec3 position;		//����λ��
		float constant;		//˥��ϵ��
		float linear;		//˥��ϵ��
		float quadratic;	//˥��ϵ��
		vec3 ambient;		//ȫ�ֹ���
		vec3 diffuse;		//������
		vec3 specular;		//����
	public:
		PointLight(const vec3 &_position, const vec3 &_ambient, const vec3 &_diffuse, const vec3 &_specular,
			const float _constant, const float _linear, const float _quadratic);
		void Bind(const GLuint program, const char* _position, const char* _ambient, const char* _diffuse, const char* _specular,
			const char* _constant, const char* _linear, const char* _quadratic) const;
	};
	//�۽���
	class SpotLight {
	private:
		vec3 position;		//����λ��
		vec3 direction;		//���շ���
		float cutOff;		//��������Ȧ��С
		float outerCutOff;	//ģ������Ȧ��С
		float constant;		//˥��ϵ��
		float linear;		//˥��ϵ��
		float quadratic;	//˥��ϵ��
		vec3 ambient;		//ȫ�ֹ���
		vec3 diffuse;		//������
		vec3 specular;		//����
	public:
		SpotLight(const vec3 &_position, const vec3 &_direction, const vec3 &_ambient, const vec3 &_diffuse, const vec3 &_specular,
			const float _constant, const float _linear, const float _quadratic, const float _cutOff, const float _outerCutOff);
		void Bind(const GLuint program, const char* _position, const char* _direction, const char* _ambient, const char* _diffuse, const char* _specular,
			const char* _constant, const char* _linear, const char* _quadratic, const char* _cutOff, const char* _outerCutOff) const;
	};
	class Material {
	private:
		GLuint diffuse;		//��������
		GLuint specular;	//������ͼ���
		float shininess;	//���淴�������Խ��Լ�⻬
	public:
		Material(const GLuint _diffuse, const GLuint _specular, const float _shininess);
		void Bind(const GLuint program, const char* _diffuse, const char* _specular, const char* _shininess) const;
	};
}
