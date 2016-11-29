#pragma once
#include "std_header.h"
#include "global_utility.h"

namespace shader
{
	class shader
	{
	private:
		GLuint program;
	public:
		shader(std::string vertex_index, std::string fragment_index);
		void use() const;
		//获取着色器编号
		const GLuint & getProgram() const;
	};
	//全局着色器列表
	const std::unordered_map <std::string, shader> & get_shader_list();
	//全局着色器初始化
	int shader_init();
}
