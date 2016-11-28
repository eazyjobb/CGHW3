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
		const GLuint & getProgram() const;
	};

	const std::unordered_map <std::string, shader> & get_shader_list();

	int shader_init();
}
