#pragma once
#include "std_header.h"
#include "global_utility.h"

namespace shader_namespace
{
	extern std::string shader_path;

	class shader
	{
	public:
		GLuint program;
		shader(std::string vertex_index, std::string fragment_index);
		void use();
	};

	extern std::unordered_map <std::string, GLuint> vertex_list, fragment_list;
	extern std::unordered_map <std::string, shader> shader_list;

	int shader_init();
	int vertex_list_init(std::ifstream &, size_t);
	int fragment_list_init(std::ifstream &, size_t);
}
