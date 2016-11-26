#include "shader.h"

namespace shader {
	std::string shader_path = "resources/shaders/";
	std::unordered_map <std::string, GLuint> vertex_list, fragment_list;
	std::unordered_map <std::string, shader> shader_list;

	int shader_init() {
		std::ifstream lin(shader_path + "shader_list.txt");
		if (false == lin.is_open()) {
			std::cout << "ERROR::SHADER::OPEN_LIST_FAILED" << std::endl;
			return -1;
		}

		std::string str; size_t n;
		lin >> str >> n; getline(lin, str);

		if (vertex_list_init(lin, n)) {
			std::cout << "ERROR::SHADER::VERTEX_INIT_FAILED" << std::endl;
			return -1;
		}

		lin >> str >> n; getline(lin, str);

		if (fragment_list_init(lin, n)) {
			std::cout << "ERROR::SHADER::FRAGMENT_INIT_FAILED" << std::endl;
			return -1;
		}

		lin >> str >> n; getline(lin, str);

		for (size_t i = 0; i < n; ++i) {
			getline(lin, str);
			auto parts = split(str, '|');
			if (parts.size() < 3 || parts[0] == "" ||
				false == vertex_list.count(parts[1]) ||
				false == fragment_list.count(parts[2])) {
				std::cout << "ERROR::SHADER::PARA_ERROR" << std::endl;
				return -1;
			}
			if (shader_list.count(parts[0]))
			{
				std::cout << "ERROR::SHADER::NAME_CONFLICT" << std::endl;
				return -1;
			}

			shader_list.insert(std::make_pair(parts[0], shader(parts[1], parts[2])));
			std::cout << shader_list.find(parts[0])->first << std::endl;
		}

		lin.close();

		/*
			释放内存
		*/
		for (auto & i : vertex_list)
			glDeleteShader(i.second);
		for (auto & i : fragment_list)
			glDeleteShader(i.second);
		vertex_list.clear();
		fragment_list.clear();

		return 0;
	}

	int vertex_list_init(std::ifstream & lin, size_t n) {
		std::string str, source;
		GLint success;
		GLchar infoLog[512];

		for (size_t i = 0; i < n; ++i) {
			getline(lin, str);

			if (vertex_list.count(str))
			{
				std::cout << "ERROR::SHADER::VERTEX::FILE_NAME_CONFLICT\n" << str + ".txt" << std::endl;
				return -1;
			}

			std::ifstream vShaderFile(shader_path + str + ".txt");
			std::stringstream vs;

			if (false == vShaderFile.is_open()) {
				std::cout << "ERROR::SHADER::VERTEX::FILE_NOT_FOUND\n" << str + ".txt" << std::endl;
				return -1;
			}

			vs << vShaderFile.rdbuf();
			source = vs.str();
			vShaderFile.close();
			const GLchar* vShaderCode = source.c_str();

			vertex_list[str] = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex_list[str], 1, &vShaderCode, NULL);
			glCompileShader(vertex_list[str]);

			glGetShaderiv(vertex_list[str], GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(vertex_list[str], 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				return -1;
			}
		}

		return 0;
	}

	int fragment_list_init(std::ifstream & lin, size_t n) {
		std::string str, source;
		GLint success;
		GLchar infoLog[512];

		for (size_t i = 0; i < n; ++i) {
			getline(lin, str);

			if (fragment_list.count(str))
			{
				std::cout << "ERROR::SHADER::FRAGMENT::FILE_NAME_CONFLICT\n" << str + ".txt" << std::endl;
				return -1;
			}

			std::ifstream fShaderFile(shader_path + str + ".txt");
			std::stringstream fs;

			if (false == fShaderFile.is_open()) {
				std::cout << "ERROR::SHADER::FRAGMENT::FILE_NOT_FOUND\n" << str + ".txt" << std::endl;
				return -1;
			}

			fs << fShaderFile.rdbuf();
			source = fs.str();
			fShaderFile.close();
			const GLchar* fShaderCode = source.c_str();

			fragment_list[str] = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment_list[str], 1, &fShaderCode, NULL);
			glCompileShader(fragment_list[str]);

			glGetShaderiv(fragment_list[str], GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(vertex_list[str], 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
				return -1;
			}
		}

		return 0;
	}

	void shader::use() {
		glUseProgram(program);
	}

	shader::shader(std::string vertex_index, std::string fragment_index) {
		GLint success;
		GLchar infoLog[512];

		program = glCreateProgram();
		glAttachShader(program, vertex_list[vertex_index]);
		glAttachShader(program, fragment_list[fragment_index]);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		std::cout << vertex_index << ' ' << fragment_index << std::endl;
	}
}
