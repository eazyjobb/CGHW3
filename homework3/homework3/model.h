#pragma once
#include "std_header.h"
#include "texture.h"
#include "light.h"

namespace model {
	class Material {
	private:
		GLuint diffuse;		//漫反射编号
		GLuint specular;	//光照贴图编号
		float shininess;	//镜面反射参数：越大约光滑
	public:
		Material(const GLuint _diffuse, const GLuint _specular, const float _shininess);
		void Bind(const GLuint program, const char* _diffuse, const char* _specular, const char* _shininess) const;
	};
}
